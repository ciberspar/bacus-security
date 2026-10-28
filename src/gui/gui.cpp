
#include "protector.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <QString>
#include <vector>

namespace {

QString format_results(
    const std::vector<bacus::CheckResult>& results
) {
    QString text;

    for (const auto& item : results) {
        text += "[" + QString::fromStdString(item.status) +
                "] " + QString::fromStdString(item.name) +
                "\n" + QString::fromStdString(item.details) +
                "\n\n";
    }

    return text;
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Bacus Protector - Purple Team");
    window.resize(680, 500);

    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);

    auto* title = new QLabel(
        "BACUS PROTECTOR | Purple Team | " +
        QString::fromStdString(bacus::version()),
        central
    );

    auto* output = new QTextEdit(central);
    output->setReadOnly(true);
    output->setPlainText(
        "Centro de seguridad\n"
        "Selecciona una herramienta.\n"
        "Las comprobaciones pendientes se indicarán como tales."
    );

    auto* file_button =
        new QPushButton("Analizar archivo", central);
    auto* system_button =
        new QPushButton("Auditar sistema", central);
    auto* tools_button =
        new QPushButton("Inventario de herramientas", central);
    auto* ports_button =
        new QPushButton("Auditoría de red/puertos", central);

    layout->addWidget(title);
    layout->addWidget(file_button);
    layout->addWidget(system_button);
    layout->addWidget(tools_button);
    layout->addWidget(ports_button);
    layout->addWidget(output);

    QObject::connect(
        file_button, &QPushButton::clicked, &window,
        [&window, output]() {
            const QString path = QFileDialog::getOpenFileName(
                &window, "Seleccionar archivo para analizar"
            );

            if (path.isEmpty()) return;

            output->setPlainText(format_results(
                bacus::scan_file(path.toStdString())
            ));
        }
    );

    QObject::connect(
        system_button, &QPushButton::clicked, &window,
        [output]() {
            output->setPlainText(
                format_results(bacus::audit_system())
            );
        }
    );

    QObject::connect(
        tools_button, &QPushButton::clicked, &window,
        [output]() {
            output->setPlainText(
                format_results(bacus::audit_tools())
            );
        }
    );

    QObject::connect(
        ports_button, &QPushButton::clicked, &window,
        [output]() {
            output->setPlainText(
                format_results(bacus::audit_ports())
            );
        }
    );

    window.setCentralWidget(central);
    window.show();

    return app.exec();
}
