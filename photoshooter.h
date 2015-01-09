#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>

#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QSpinBox>
#include <QSpacerItem>
#include <QThread>
#include <QProcess>
#include <QByteArray>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class PhotoShooter : public QMainWindow
{
    Q_OBJECT

    class Shooter: public QThread {

    public:
        Shooter(QObject* parentO, PhotoShooter* parent);
        ~Shooter();

    protected:
        void run() Q_DECL_OVERRIDE;

    private:
        PhotoShooter* parent;

    };

public:
    PhotoShooter();
    ~PhotoShooter();
    bool loadFile(const QString &);
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

private:
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    int  takePicture();
    void setStautsbarText(QString txt);

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    bool running;
    Shooter* shooter;
    QString mydir;

    QLabel* statusbar;
    QSpinBox* timeintervall;
    QPushButton* multipleButton;

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
