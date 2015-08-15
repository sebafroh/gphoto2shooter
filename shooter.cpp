#include <QtWidgets>
#include <unistd.h>

#include "photoshooter.h"
#include "shooter.h"

/** Initializes the Thread takes the pictures */
Shooter::Shooter(PhotoShooter *parent) : QThread(parent),  photoShooter(parent) {
}

/** Destroys the object */
Shooter::~Shooter() {
    wait();
}

/** Takes a picture every X seconds */
void Shooter::run() {
    while(photoShooter->running) {
        for(int i = photoShooter->timeintervall->value(); i > 1  && photoShooter->running; --i) {
            emit changeMessage(tr("Time to next picture: %1 seconds").arg(QString::number(i)));
            sleep(1);
        }

        if(photoShooter->running) {
            emit changeMessage(tr("CHEESE"));
            sleep(1);
        }

        if(photoShooter->running) {
            emit takePicture();
            sleep(1);
        }
    }
}
