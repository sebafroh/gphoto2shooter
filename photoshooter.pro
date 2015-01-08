QT += widgets
qtHaveModule(printsupport): QT += printsupport

QMAKE_CXX = clang++
QMAKE_LINK= clang++


HEADERS       = photoshooter.h
SOURCES       = photoshooter.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/photoshooter
INSTALLS += target

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}
