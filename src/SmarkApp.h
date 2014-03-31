#ifndef SMARK_OPTION_H
#define SMARK_OPTION_H

#include <QString>
#include <QDir>

enum SmarkMode {
    EditMode    = 0,
    ReadMode    = 1,
    PreviewMode = 2
};

class SmarkApp
{
    friend SmarkApp& gApp(void);

private:

    SmarkApp(void);

public:

    QDir          BinDir;
    QDir          ShareDir;
    QString       StartPath;
    QDir          StartDir;
    QString       CSSPath;
    QString       Template;
    QString       CSS;
    QString       MathJaxPath;
    SmarkMode     CurrentMode;
};

SmarkApp& gApp(void);

void gInitApp(int argc, char** argv);

#endif // SMARK_OPTION_H
