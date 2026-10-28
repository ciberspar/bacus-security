
#include "protector.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

#ifdef __linux__
#include <sys/stat.h>
#endif

namespace bacus {
namespace {

namespace fs = std::filesystem;

constexpr std::size_t BLOCK_SIZE = 64 * 1024;

const std::string EICAR_SIGNATURE =
    "X5O!P%@AP[4\\PZX54(P^)7CC)7}$"
    "EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";

bool executable_available(const std::string& name) {
#ifdef _WIN32
    const char* env_path = std::getenv("PATH");
    if (!env_path) return false;

    const std::string paths(env_path);
    const std::string extensions = ".exe;.com;.bat;.cmd;";
    std::size_t start = 0;

    while (start <= paths.size()) {
        const auto end = paths.find(';', start);
        const std::string dir = paths.substr(
            start, end == std::string::npos
                ? std::string::npos : end - start
        );

        for (const auto& ext : {std::string(".exe"),
                                std::string(".com"),
                                std::string("")}) {
            const fs::path candidate =
                fs::path(dir) / (name + ext);
            std::error_code ec;
            if (fs::is_regular_file(candidate, ec) && !ec) {
                return true;
            }
        }

        if (end == std::string::npos) break;
        start = end + 1;
    }
    (void)extensions;
    return false;
#else
    const char* env_path = std::getenv("PATH");
    if (!env_path) return false;

    const std::string paths(env_path);
    std::size_t start = 0;

    while (start <= paths.size()) {
        const auto end = paths.find(':', start);
        const std::string dir = paths.substr(
            start, end == std::string::npos
                ? std::string::npos : end - start
        );

        const fs::path candidate =
            (dir.empty() ? fs::path(".") : fs::path(dir)) / name;

        std::error_code ec;
        const auto perms = fs::status(candidate, ec).permissions();

        if (!ec && fs::is_regular_file(candidate, ec) && !ec) {
            using P = fs::perms;
            const auto executable_bits =
                P::owner_exec | P::group_exec | P::others_exec;

            if ((perms & executable_bits) != P::none) {
                return true;
            }
        }

        if (end == std::string::npos) break;
        start = end + 1;
    }

    return false;
#endif
}

} // namespace

std::string version() {
    return "0.3.0";
}

std::vector<CheckResult> audit_system() {
    std::vector<CheckResult> results;

#ifdef __linux__
    results.push_back({
        "Sistema operativo",
        "INFO",
        "Linux detectado"
    });

    struct stat info {};

    if (::stat("/tmp", &info) == 0) {
        const bool writable_by_all =
            (info.st_mode & S_IWOTH) != 0;
        const bool sticky =
            (info.st_mode & S_ISVTX) != 0;

        if (writable_by_all && !sticky) {
            results.push_back({
                "Permisos de /tmp",
                "REVISAR",
                "Escribible por todos y sin sticky bit"
            });
        } else {
            results.push_back({
                "Permisos de /tmp",
                "INFO",
                "Comprobación básica superada; auditoría limitada"
            });
        }
    } else {
        results.push_back({
            "Permisos de /tmp",
            "ERROR",
            "No se pudo consultar /tmp"
        });
    }
#elif defined(_WIN32)
    results.push_back({
        "Sistema operativo", "INFO",
        "Windows detectado; auditoría específica pendiente"
    });
#elif defined(__APPLE__)
    results.push_back({
        "Sistema operativo", "INFO",
        "Apple OS detectado; auditoría específica pendiente"
    });
#else
    results.push_back({
        "Sistema operativo", "INFO",
        "Sistema no identificado"
    });
#endif

    results.push_back({
        "Auditoría completa",
        "PENDIENTE",
        "No se han comprobado todas las configuraciones del sistema"
    });

    return results;
}

std::vector<CheckResult> audit_ports() {
    return {{
        "Auditoría de red",
        "PENDIENTE",
        "El inventario de puertos todavía no está implementado. "
        "No se ha realizado ningún escaneo."
    }};
}

std::vector<CheckResult> audit_tools() {
    const std::vector<std::string> tools = {
        "clamscan",
        "freshclam",
        "yara",
        "nmap",
        "tshark",
        "openssl",
        "osqueryi"
    };

    std::vector<CheckResult> results;

    for (const auto& tool : tools) {
        const bool found = executable_available(tool);

        results.push_back({
            tool,
            found ? "DISPONIBLE" : "NO ENCONTRADA",
            found
                ? "Se encontró una ruta ejecutable en PATH; "
                  "todavía no se ha probado su funcionamiento"
                : "No se encontró en PATH"
        });
    }

    return results;
}

std::vector<CheckResult> scan_file(const std::string& path) {
    if (path.empty()) {
        return {{
            "Archivo", "ERROR", "La ruta está vacía"
        }};
    }

    std::error_code ec;
    const fs::path file_path(path);

    if (!fs::is_regular_file(file_path, ec) || ec) {
        return {{
            "Archivo", "ERROR",
            "No es un archivo normal accesible"
        }};
    }

    const auto expected_size = fs::file_size(file_path, ec);
    if (ec) {
        return {{
            "Archivo", "ERROR",
            "No se pudo consultar el tamaño"
        }};
    }

    std::ifstream input(file_path, std::ios::binary);
    if (!input) {
        return {{
            "Archivo", "ERROR",
            "No se pudo abrir el archivo para lectura"
        }};
    }

    std::array<char, BLOCK_SIZE> buffer {};
    std::string overlap;
    overlap.reserve(EICAR_SIGNATURE.size());

    std::uintmax_t bytes_read = 0;
    bool eicar_found = false;

    while (input) {
        input.read(
            buffer.data(),
            static_cast<std::streamsize>(buffer.size())
        );

        const auto count = input.gcount();
        if (count <= 0) break;

        bytes_read += static_cast<std::uintmax_t>(count);

        std::string chunk = overlap;
        chunk.append(buffer.data(), static_cast<std::size_t>(count));

        if (chunk.find(EICAR_SIGNATURE) != std::string::npos) {
            eicar_found = true;
            break;
        }

        const std::size_t keep = EICAR_SIGNATURE.size() - 1;
        if (chunk.size() > keep) {
            overlap.assign(chunk.end() - keep, chunk.end());
        } else {
            overlap = chunk;
        }
    }

    if (input.bad()) {
        return {{
            "Análisis de archivo", "INCOMPLETO",
            "Error de lectura. No se puede considerar verificado."
        }};
    }

    if (eicar_found) {
        return {{
            "Firma de prueba EICAR", "DETECTADO",
            "Firma antivirus de prueba encontrada. No demuestra "
            "que se haya detectado malware real."
        }};
    }

    if (bytes_read != expected_size) {
        return {{
            "Análisis de archivo", "INCOMPLETO",
            "El tamaño leído no coincide con el tamaño inicial."
        }};
    }

    return {{
        "Análisis de archivo", "SIN INDICIOS CONOCIDOS",
        "Se leyó el archivo completo (" +
        std::to_string(bytes_read) +
        " bytes) y no se encontró la firma EICAR. "
        "No se ha realizado una detección antivirus completa."
    }};
}

} // namespace bacus
