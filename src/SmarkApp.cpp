#include <QDebug>
#include <QStringList>
#include <QFileInfo>
#include "SmarkApp.h"
#include "SmarkFileIO.h"

SmarkApp::SmarkApp(void)
    : CurrentMode(ReadMode) {
    /* nothing need to do */
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

    // get bin path and load css ...
    QFileInfo bin_file(Arg[0]);
    gApp().BinDir      = bin_file.dir();
    gApp().ShareDir    = QDir(bin_file.path() + QString("/share/"));
    gApp().CSSPath     = bin_file.path() + QString("/share/smark.css");
    gLoadTextFile(gApp().CSSPath,     &(gApp().CSS));

    QString TemplatePath = bin_file.path() + QString("/share/template.html");
    gLoadTextFile(TemplatePath, &(gApp().Template));

    QString MathJaxPath = bin_file.path() + QString("/share/mathjax.config");
    gLoadTextFile(MathJaxPath, &(gApp().MathJaxPath));

    // markdown file path
    if(argc > 1) {
        QFileInfo start_file(Arg[1]);
        gApp().StartDir  = start_file.dir();
        gApp().StartPath = start_file.filePath();
    }
}
