QT += widgets
qtHaveModule(printsupport): QT += printsupport

QMAKE_CXX  = clang++
QMAKE_LINK = clang++

CONFIG += qt debug

TRANSLATIONS  = photoshooter_de.ts

HEADERS       = photoshooter.h \
		shooter.h
SOURCES       = photoshooter.cpp \
		shooter.cpp \
                main.cpp

# install
target.path = /usr/bin/
INSTALLS += target

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}
