
#include "protector.hpp"

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <QString>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Bacus Protector");
    window.resize(620, 420);

    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);

    auto* title = new QLabel(
        "Bacus Protector " + QString::fromStdString(bacus::version()),
        central
    );

    auto* output = new QTextEdit(central);
    output->setReadOnly(true);
    output->setPlainText(
        "Prototipo de seguridad\n"
        "Selecciona una comprobacion.\n"
        "El analisis antivirus aun no esta implementado."
    );

    auto* system_button = new QPushButton(
        "Auditar sistema", central
    );
    auto* ports_button = new QPushButton(
        "Auditar puertos", central
    );

    layout->addWidget(title);
    layout->addWidget(system_button);
    layout->addWidget(ports_button);
    layout->addWidget(output);

    QObject::connect(system_button, &QPushButton::clicked,
                     &window, [output]() {
        QString text;

        for (const auto& item : bacus::audit_system()) {
            text += "[" + QString::fromStdString(item.status) +
                    "] " + QString::fromStdString(item.name) +
                    "\n" + QString::fromStdString(item.details) +
                    "\n\n";
        }

        output->setPlainText(text);
    });

    QObject::connect(ports_button, &QPushButton::clicked,
                     &window, [output]() {
        QString text;

        for (const auto& item : bacus::audit_ports()) {
            text += "[" + QString::fromStdString(item.status) +
                    "] " + QString::fromStdString(item.name) +
                    "\n" + QString::fromStdString(item.details) +
                    "\n\n";
        }

        output->setPlainText(text);
    });

    window.setCentralWidget(central);
    window.show();

    return app.exec();
}
