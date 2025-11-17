#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style/style.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(file.readAll());
        a.setStyleSheet(style);
        file.close();
    }

    try
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    catch (const std::exception &e)
    {
        qDebug() << "Exception:" << e.what();
        return -1;
    }
}
