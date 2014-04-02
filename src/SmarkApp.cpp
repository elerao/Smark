#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include "SmarkApp.h"
#include "SmarkFileIO.h"

SmarkApp::SmarkApp(void) {
    /* nothing need to do */
}

SmarkApp::~SmarkApp(void) {
    gSaveTextFile(CSSPath, CSS);
    gSaveConfigFile(ConfigPath, Config);
}

SmarkApp& gApp(void) {
    static SmarkApp app;
    return app;
}

void gInitApp(int argc, char** argv) {
    // build argument list
    QStringList Arg;
    for(int i=0; i<argc; ++i)
        Arg.append(QString::fromLocal8Bit(argv[i]));

    // binary file path
    QFileInfo binaryFileInfo(Arg[0]);
    gApp().BinaryDir = binaryFileInfo.dir();

    // markdown file path
    if(argc > 1) {
        QFileInfo startFileInfo(Arg[1]);
        gApp().StartDir  = startFileInfo.dir();
        gApp().StartPath = startFileInfo.filePath();
    }

#ifdef _WIN32
    gApp().CacheDir   = QDir(binaryFileInfo.path());
    gApp().CSSPath    = binaryFileInfo.path() + "/share/smark.css";
    gApp().ConfigPath = binaryFileInfo.path() + "/share/smark.config";
#else
    // the users may not have the right to write or read a file to the
    // dir $$binaryFileInfo.dir()$$/share if they are not a root user
    gApp().CacheDir   = QDir("~/.tmp/");
    gApp().CSSPath    = "~/.share/smark.css";
    gApp().ConfigPath = "~/.share/smark.config";
#endif

    gLoadTextFile(gApp().CSSPath, &(gApp().CSS));
    gApp().Config = gLoadConfigFile(gApp().ConfigPath);
}
