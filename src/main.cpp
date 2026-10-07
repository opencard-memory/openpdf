#include <QApplication>
#include <QFile>
#include <QFont>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("OpenPDF Office");
    QApplication::setOrganizationName("OpenPDFOffice");
    QApplication::setApplicationVersion("0.1.0");
    app.setFont(QFont("Segoe UI", 10));

    MainWindow window;
    window.show();
    if (argc > 1) window.openPath(QString::fromLocal8Bit(argv[1]));
    return app.exec();
}
