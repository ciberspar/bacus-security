
#include "protector.hpp"

#include <iostream>
#include <string>
#include <vector>

int run_tui();

namespace {

void print_results(
    const std::vector<bacus::CheckResult>& results
) {
    for (const auto& item : results) {
        std::cout << "[" << item.status << "] "
                  << item.name << "\n"
                  << "  " << item.details << "\n\n";
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return run_tui();
    }

    const std::string command = argv[1];

    if (command == "--version" || command == "version") {
        std::cout << "Bacus Protector " << bacus::version() << '\n';
        return 0;
    }

    if (command == "--help" || command == "help") {
        std::cout
            << "Bacus Protector " << bacus::version() << "\n\n"
            << "Uso:\n"
            << "  bacus-protector-tui\n"
            << "  bacus-protector-tui --version\n"
            << "  bacus-protector-tui scan ARCHIVO\n"
            << "  bacus-protector-tui audit\n"
            << "  bacus-protector-tui tools\n"
            << "  bacus-protector-tui ports\n";
        return 0;
    }

    if (command == "scan" && argc == 3) {
        const auto results = bacus::scan_file(argv[2]);
        print_results(results);

        for (const auto& result : results) {
            if (result.status == "ERROR" ||
                result.status == "INCOMPLETO" ||
                result.status == "DETECTADO") {
                return 1;
            }
        }

        return 0;
    }

    if (command == "audit" && argc == 2) {
        print_results(bacus::audit_system());
        return 0;
    }

    if (command == "tools" && argc == 2) {
        print_results(bacus::audit_tools());
        return 0;
    }

    if (command == "ports" && argc == 2) {
        print_results(bacus::audit_ports());
        return 0;
    }

    std::cerr << "Comando no reconocido. Usa --help.\n";
    return 2;
}
