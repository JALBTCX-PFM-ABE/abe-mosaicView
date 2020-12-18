contains(QT_CONFIG, opengl): QT += opengl
QT += 
RC_FILE = mosaicView.rc
RESOURCES = icons.qrc
INCLUDEPATH += /c/PFM_ABEv7.0.0_Win64/include
LIBS += -L /c/PFM_ABEv7.0.0_Win64/lib -lnvutility -lBinaryFeatureData -lgdal -lxml2 -lpoppler -liconv
DEFINES += WIN32 NVWIN3X
CONFIG += console

#
# The following line is included so that the contents of acknowledgments.hpp will be available for translation
#

HEADERS += /c/PFM_ABEv7.0.0_Win64/include/acknowledgments.hpp

QMAKE_LFLAGS += 
######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 14:24:20 2020
######################################################################

TEMPLATE = app
TARGET = mosaicView
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += editFeature.hpp \
           editFeatureHelp.hpp \
           featureTypes.hpp \
           mosaicView.hpp \
           mosaicViewDef.hpp \
           mosaicViewHelp.hpp \
           prefs.hpp \
           prefsHelp.hpp \
           screenshot.hpp \
           screenshotHelp.hpp \
           version.hpp
SOURCES += editFeature.cpp \
           env_in_out.cpp \
           get_geotiff.cpp \
           main.cpp \
           mosaicView.cpp \
           overlayFlag.cpp \
           prefs.cpp \
           screenshot.cpp \
           set_defaults.cpp
RESOURCES += icons.qrc
TRANSLATIONS += mosaicView_xx.ts