#ifndef SMARK_OPTION_H
#define SMARK_OPTION_H

#include <QString>
#include <QDir>

enum SmarkMode {
    EditMode    = 0,
    ReadMode    = 1,
    PreviewMode = 2
};

struct SmarkConfig
{
public:
    SmarkMode StartMode;
    bool      ShowMenu;
    bool      ShowToolbar;
public:
    SmarkConfig(void)
        : StartMode(ReadMode),
          ShowMenu(true),
          ShowToolbar(true) {
        /*nothing need to do*/
    }
};

class SmarkApp
{
    friend SmarkApp& gApp(void);

private:

    SmarkApp(void);
    ~SmarkApp(void);

public:

    QDir BinaryDir;
    QDir StartDir;
    QDir CacheDir;

    QString StartPath;
    QString CSSPath;
    QString ConfigPath;

    QString       CSS;
    SmarkConfig   Config;
};

SmarkApp& gApp(void);

void gInitApp(int argc, char** argv);

#endif // SMARK_OPTION_H
