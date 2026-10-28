
#include "protector.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

void show_results(
    const std::vector<bacus::CheckResult>& results
) {
    for (const auto& result : results) {
        std::cout << "\n[" << result.status << "] "
                  << result.name << '\n'
                  << "  " << result.details << '\n';
    }
}

void pause() {
    std::cout << "\nPresiona ENTER para continuar...";
    std::string line;
    std::getline(std::cin, line);
}

} // namespace

int run_tui() {
    while (true) {
        std::cout
            << "\n================================\n"
            << "       BACUS PROTECTOR\n"
            << "       PURPLE TEAM LAB\n"
            << "================================\n"
            << "Version " << bacus::version() << "\n\n"
            << "1. Analizar archivo\n"
            << "2. Auditar sistema\n"
            << "3. Inventario de herramientas\n"
            << "4. Auditoría de red/puertos\n"
            << "5. Información\n"
            << "0. Salir\n\n"
            << "Opcion: ";

        std::string choice;
        if (!std::getline(std::cin, choice)) {
            return 0;
        }

        if (choice == "0") {
            return 0;
        }

        if (choice == "1") {
            std::string path;
            std::cout << "Ruta del archivo: ";

            if (!std::getline(std::cin, path)) {
                return 0;
            }

            show_results(bacus::scan_file(path));
        } else if (choice == "2") {
            show_results(bacus::audit_system());
        } else if (choice == "3") {
            show_results(bacus::audit_tools());
        } else if (choice == "4") {
            show_results(bacus::audit_ports());
        } else if (choice == "5") {
            std::cout
                << "\nBacus Protector " << bacus::version() << '\n'
                << "Plataforma de ciberseguridad Purple Team.\n"
                << "Los módulos pendientes se identifican claramente.\n"
                << "No es todavía un antivirus completo.\n";
        } else {
            std::cout << "Opcion desconocida.\n";
        }

        pause();
    }
}
