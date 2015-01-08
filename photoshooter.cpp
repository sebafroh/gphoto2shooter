#include <QtWidgets>
#include <iostream>
#include <unistd.h>

#include "photoshooter.h"

/** Initialises the Threadthat does the taking of pictures
*/
PhotoShooter::Shooter::Shooter(QObject* parentO, PhotoShooter* parent) : QThread(parentO),  parent(parent)
{
}

/** Destroys the object*/
PhotoShooter::Shooter::~Shooter()
{
    wait();
}

/** Takes a picture al X seconds */
void PhotoShooter::Shooter::run() {
    while (parent->running) {
        for (int i = parent->timeintervall->value(); i > 1  && parent->running; --i) {
            parent->cheese->setText(QString("Time to next picture: ").append(QString::number(i)).append(" seconds"));
            sleep(1);
        }
        parent->cheese->setText("CHEESE!");
        sleep(1);
        parent->takePicture();
    }
}

/** Initialises the Program */
PhotoShooter::PhotoShooter() : running(false)
{
    QWidget* mainWidget = new QWidget();

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    QPushButton* singleButton = new QPushButton("Take a single picture",this);
    connect(singleButton, SIGNAL(clicked()), this, SLOT(singleShot()));

    multipleButton = new QPushButton("START",this);
    connect(multipleButton, SIGNAL(clicked()), this, SLOT(multipleShot()));

    QLabel* tid = new QLabel("Time between pictures in s", this);

    cheese = new QLabel("press start to begin taking pictures",this);
    cheese->setAlignment(Qt::AlignCenter);

    timeintervall = new QSpinBox(this);
    timeintervall->setValue(5);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(scrollArea,          0, 0, 1, 7);
    mainLayout->addWidget(singleButton,        1, 0);
    mainLayout->addWidget(multipleButton,      1, 1);
    mainLayout->addWidget(cheese,              1, 2);
    mainLayout->addItem(new QSpacerItem(1,1),  1, 3);
    mainLayout->addWidget(tid,                 1, 4);
    mainLayout->addItem(new QSpacerItem(1,1),  1, 5);
    mainLayout->addWidget(timeintervall,       1, 6);
    mainLayout->setColumnStretch(0,4);
    mainLayout->setColumnStretch(1,4);
    mainLayout->setColumnStretch(2,8);
    mainLayout->setColumnStretch(3,2);
//     mainLayout->setColumnStretch(4,1);
    mainLayout->setColumnStretch(6,1);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    createActions();
    createMenus();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    shooter = new Shooter(this,this);

    mydir = "~";
}

/**Destroys the Program data*/
PhotoShooter::~PhotoShooter()
{
    shooter->terminate();
    delete shooter;
}

/** Loads a File from disk an shows it in the main Window*/
bool PhotoShooter::loadFile(const QString& fileName)
{
    QImage image(fileName);
    std::cout << "Loading file: " << fileName.toStdString() << std::endl;
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    setWindowFilePath(fileName);
    return true;
}

/**Toggles Mode to take multiple Pictures when Button is pressed*/
void PhotoShooter::multipleShot()
{
    running = !running;
    if (running) {
        multipleButton->setText("STOP");
        shooter->start();
    } else {
        multipleButton->setText("START");
        cheese->setText("press start to begin taking pictures");
        shooter->terminate();
    }
}

/**Takes one picture when Button is pressed*/
void PhotoShooter::singleShot()
{
    takePicture();
}

/** Takes one Picture*/
int PhotoShooter::takePicture()
{

    QString path;
    QString command;
    QDateTime now = QDateTime::currentDateTime();


    path.append(mydir).append("/img_").append(now.toString("yyyy.mm.dd_hh:mm:ss")).append(".JPG");
    command.append("gphoto2 --capture-image-and-download --filename ").append(path);

    std::cout << "Taking picture" << std::endl;
    cheese->setText("Please wait while data will be transferred...");

    QProcess* getData = new QProcess(this);
    getData->start(command);
    getData->waitForFinished();

    int returnValue = getData->exitCode();

    if ( returnValue == 0) {
        loadFile(path);
        cheese->setText(QString("Picture taken. Image saved to ").append(path));
    } else {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("No camera found<br><br>Please check connection an make sure the camera is ready to take pictures<br><br>").append(getData->readAll()) );
        cheese->setText("Error taking Picture ");
    }

    return returnValue;
}

/** Selects the Folder the Images are written to*/
void PhotoShooter::selectFolder()
{
    std::cout << "Folder!" <<std::endl;

    QFileDialog dialog(this, tr("Open File"), QDir::currentPath());

    dialog.setFileMode(QFileDialog::Directory);

    if (dialog.exec() == QDialog::Accepted) {
        std::cout << "Choosing folder " << dialog.selectedFiles().first().toStdString() << std::endl;
        mydir = dialog.selectedFiles().first();
    }
    else
        std::cout << "Esc" << std::endl;

}

/**Shows the Information gphoto2 shows about connected cameras*/
void PhotoShooter::showCamera()
{
    QProcess* getData = new QProcess(this);
    getData->start("gphoto2 --auto-detect");
    getData->waitForFinished();
    QByteArray result = getData->readAll();
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                             result);
}

/**Zooms in to the picture*/
void PhotoShooter::zoomIn()
{
    scaleImage(1.25);
}

/**Zooms out of the picture*/
void PhotoShooter::zoomOut()
{
    scaleImage(0.8);
}

/** Sets zommfaktor to 1*/
void PhotoShooter::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

/**Scales the picture so it fits into the window*/
void PhotoShooter::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

/**Shows a about-Message*/
void PhotoShooter::about()
{
    QMessageBox::about(this, tr("About GPhoto2 Shooter"),
                       tr("<p>The <b>GPhoto2 Shooter</b> is a simple programm to take several photos from a camera, saves them directly to the computer and shows the last taken picture on the screen.<br><br>"
                          "This may be usefull to take selfportraits or if you want to see the results imediately on your screen. Have Fun...<br><br>"
			  "Version 1.0 released by Sebastian Frohn &lt;unrath@unterderbruecke.de&gt; at 08.01.2015"));
}

/** Creates the Actions used by menu an buttons*/
void PhotoShooter::createActions()
{

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    showCameraAct= new QAction(tr("Show &available Cameras"), this);
    showCameraAct->setShortcut(tr("Ctrl+A"));
    connect(showCameraAct, SIGNAL(triggered()), this, SLOT(showCamera()));

    singleShotAct = new QAction(tr("Take &single picture"), this);
    singleShotAct->setShortcut(tr("Ctrl+S"));
    connect(singleShotAct, SIGNAL(triggered()), this, SLOT(singleShot()));

    multipleShotAct = new QAction(tr("Take &multiple pictures"), this);
    multipleShotAct->setShortcut(tr("Ctrl+M"));
    connect(multipleShotAct, SIGNAL(triggered()), this, SLOT(multipleShot()));

    selectFolderAct = new QAction(tr("S&elect Folder"), this);
    selectFolderAct->setShortcut(tr("Ctrl+E"));
    connect(selectFolderAct, SIGNAL(triggered()), this, SLOT(selectFolder()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+N"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

/** Creates the menus*/
void PhotoShooter::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);

    fileMenu->addAction(showCameraAct);
    fileMenu->addAction(selectFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(singleShotAct);
    fileMenu->addAction(multipleShotAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}


/**Toggles the zooming-Actions when fit-to-window-moode ist toggled*/
void PhotoShooter::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

/**scales the image*/
void PhotoShooter::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

/**Adjustes the scrollbar*/
void PhotoShooter::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
