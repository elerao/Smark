#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"
#include "Smark/SmkMainWindow.h"

int main(int argc, char** argv)
{
    SmkApp app(argc, argv);

#if (QT_VERSION < 0x050000)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
#endif

#ifdef SMK_WINDOWS_PLATFORM
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif // SMK_WINDOWS_PLATFORM

    SmkMainWindow window(NULL);
    if(argc > 1)
        window.openMarkdownFile(QString::fromLocal8Bit(argv[1]));
    window.showNormal();

    return app.exec();
}
