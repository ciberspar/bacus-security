
#include "protector.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace bacus {

std::string version() {
    return "0.1.0";
}

std::vector<CheckResult> audit_system() {
    std::vector<CheckResult> results;

#ifdef __linux__
    results.push_back({
        "Sistema operativo",
        "INFO",
        "Linux; auditoria completa pendiente"
    });
#elif defined(_WIN32)
    results.push_back({
        "Sistema operativo",
        "INFO",
        "Windows; auditoria completa pendiente"
    });
#elif defined(__APPLE__)
    results.push_back({
        "Sistema operativo",
        "INFO",
        "Apple OS; auditoria completa pendiente"
    });
#else
    results.push_back({
        "Sistema operativo",
        "INFO",
        "Sistema no identificado"
    });
#endif

    results.push_back({
        "Auditoria de seguridad",
        "PENDIENTE",
        "Todavia no se comprobaron todas las configuraciones"
    });

    return results;
}

std::vector<CheckResult> audit_ports() {
    return {{
        "Puertos de red",
        "PENDIENTE",
        "El analizador de puertos aun no esta implementado"
    }};
}

std::vector<CheckResult> scan_file(const std::string& path) {
    namespace fs = std::filesystem;

    std::error_code ec;
    const fs::path file_path(path);

    if (!fs::exists(file_path, ec) || ec) {
        return {{
            "Archivo",
            "ERROR",
            "El archivo no existe o no se puede consultar"
        }};
    }

    if (!fs::is_regular_file(file_path, ec) || ec) {
        return {{
            "Archivo",
            "ERROR",
            "La ruta no corresponde a un archivo normal"
        }};
    }

    const auto size = fs::file_size(file_path, ec);

    if (ec) {
        return {{
            "Archivo",
            "ERROR",
            "No se pudo consultar el tamano"
        }};
    }

    std::ifstream input(file_path, std::ios::binary);

    if (!input) {
        return {{
            "Archivo",
            "ERROR",
            "No se pudo abrir el archivo para leerlo"
        }};
    }

    return {{
        "Archivo",
        "NO ANALIZADO",
        "Lectura posible; tamano: " + std::to_string(size) +
        " bytes. No se ha realizado un analisis antivirus."
    }};
}

} // namespace bacus
