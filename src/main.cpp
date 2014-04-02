#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include "GuiWindowSmark.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
#ifdef _WIN32
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
#endif //_WIN32
    ::gInitApp(argc, argv);

    GuiWindowSmark wnd(NULL);
    if(argc > 1)
        wnd.openMarkdownFile(gApp().StartPath);
    wnd.show();

    return app.exec();
}
