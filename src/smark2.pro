# Build Configure ##############################################################

TEMPLATE = app
TARGET   = smark
QT       += widgets webkit webkitwidgets
INCLUDEPATH += . src
DEPENDPATH  += . src
# Input File ###################################################################

HEADERS += \
    Smark/SmkConfig.h \
    Smark/SmkApp.h \
    Smark/SmkIO.h \
    Smark/SmkMarkParser.h \
    Smark/SmkMarkHighlighter.h \
    Smark/SmkMarkWidget.h \
    Smark/SmkHtmlWidget.h \
    Smark/SmkOptionDialog.h \
    Smark/SmkBasicMarkWidget.h \
    Smark/SmkMainWindow.h \
    Smark/SmkHtmlHighlighter.h

SOURCES += \
    Smark/SmkApp.cpp \
    Smark/qMain.cpp \
    Smark/SmkMarkParser.cpp \
    Smark/SmkMarkHighlighter.cpp \
    Smark/SmkMarkWidget.cpp \
    Smark/SmkHtmlWidget.cpp \
    Smark/SmkOptionDialog.cpp \
    Smark/SmkBasicMarkWidget.cpp \
    Smark/SmkMainWindow.cpp \
    Smark/SmkHtmlHighlighter.cpp

FORMS += \
    Smark/SmkOptionDialog.ui \
    Smark/SmkMainWindow.ui

RESOURCES += \
    Smark/rc/icon.qrc


RC_FILE += Smark/rc/LOGO.rc
