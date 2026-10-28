
#include "protector.hpp"

#include <iostream>
#include <string>

int run_tui();

int main(int argc, char* argv[]) {
    if (argc > 1) {
        const std::string command = argv[1];

        if (command == "version" || command == "--version") {
            std::cout << "Bacus Protector "
                      << bacus::version() << '\n';
            return 0;
        }

        if (command == "help" || command == "--help") {
            std::cout
                << "Bacus Protector\n"
                << "Uso: bacus-protector-tui [help|version]\n";
            return 0;
        }

        std::cerr << "Comando no reconocido: " << command << '\n';
        return 2;
    }

    return run_tui();
}
