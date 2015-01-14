#ifndef SHOOTER_H
#define SHOOTER_H

#include <QThread>
#include <QString>


class PhotoShooter;

class Shooter: public QThread {
    Q_OBJECT

public:
    Shooter(PhotoShooter* photoShooter);
    ~Shooter();

signals: 
    void changeMessage(QString msg);
    void takePicture();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    PhotoShooter* photoShooter;

};

#endif