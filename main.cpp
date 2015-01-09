#include <QApplication>
#include <QTranslator>

#include "photoshooter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("photoshooter_de");
    app.installTranslator(&translator);
    QGuiApplication::setApplicationDisplayName(PhotoShooter::tr("GPhoto2 Shooter"));
    PhotoShooter photoshooter;
    photoshooter.show();
    photoshooter.loadFile("intro.png");
    photoshooter.resizeImage();
    return app.exec();
}
