#include <QApplication>
#include <QFile>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/style.qss");

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString style = styleFile.readAll();
        app.setStyleSheet(style);
        styleFile.close();
    } else {
        qDebug() << "Failed to open style.qss";
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
