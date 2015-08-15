#ifndef PHOTOSHOOTER_H
#define PHOTOSHOOTER_H

#include <QMainWindow>

#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QSpinBox>
#include <QSpacerItem>
#include <QThread>
#include <QProcess>
#include <QByteArray>

#include "shooter.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class PhotoShooter;

class PhotoShooter : public QMainWindow {
    Q_OBJECT

    friend class Shooter;

  public:
    PhotoShooter();
    ~PhotoShooter();
    bool loadFile( const QString & );
    void resizeImage();

  private slots:
    void normalSize();
    void fitToWindow();
    void showCamera();
    void singleShot();
    void multipleShot();
    void selectFolder();
    void zoomIn();
    void zoomOut();
    void about();

    void setStatusbarText( QString message );
    int  takePicture();

  private:
    void createLayout();
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage( double factor );
    void adjustScrollBar( QScrollBar *scrollBar, double factor );

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    bool running;
    Shooter *shooter;
    QString mydir;

    QLabel *statusbar, *tid;
    QSpinBox *timeintervall;
    QPushButton *multipleButton, *singleButton;

    QAction *exitAct;
    QAction *singleShotAct;
    QAction *multipleShotAct;
    QAction *showCameraAct;
    QAction *selectFolderAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
};

#endif
