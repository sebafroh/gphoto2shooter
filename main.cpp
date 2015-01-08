#include <QApplication>


#include "photoshooter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(PhotoShooter::tr("GPhoto2 Shooter"));
    PhotoShooter photoshooter;
    photoshooter.show();
    photoshooter.loadFile("intro.png");
    photoshooter.resizeImage();
    return app.exec();
}
