
#include "protector.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

void show_results(const std::vector<bacus::CheckResult>& results) {
    for (const auto& result : results) {
        std::cout << "\n[" << result.status << "] "
                  << result.name << "\n"
                  << "  " << result.details << "\n";
    }
}

void pause() {
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );
}

} // namespace

int run_tui() {
    while (true) {
        std::cout << "\n"
                  << "============================\n"
                  << "      BACUS PROTECTOR\n"
                  << "============================\n"
                  << "Version " << bacus::version() << "\n\n"
                  << "1. Analizar un archivo\n"
                  << "2. Auditar el sistema\n"
                  << "3. Auditar puertos\n"
                  << "4. Informacion\n"
                  << "0. Salir\n\n"
                  << "Opcion: ";

        int option = -1;

        if (!(std::cin >> option)) {
            if (std::cin.eof()) {
                return 0;
            }

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );
            std::cout << "Opcion invalida.\n";
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        if (option == 0) {
            return 0;
        }

        if (option == 1) {
            std::string path;
            std::cout << "Ruta del archivo: ";

            if (!std::getline(std::cin, path)) {
                return 0;
            }

            show_results(bacus::scan_file(path));
        } else if (option == 2) {
            show_results(bacus::audit_system());
        } else if (option == 3) {
            show_results(bacus::audit_ports());
        } else if (option == 4) {
            std::cout
                << "\nBacus Protector " << bacus::version() << "\n"
                << "Prototipo en desarrollo.\n"
                << "No sustituye a un antivirus.\n";
        } else {
            std::cout << "Opcion desconocida.\n";
        }

        pause();
    }
}
