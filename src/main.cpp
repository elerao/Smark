#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include "GuiWindowSmark.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ::gInitApp(argc, argv);
//    qDebug() << gApp().Arg << endl
//             << gApp().BinDir << endl
//             << gApp().ShareDir << endl
//             << gApp().CSS << endl;

    GuiWindowSmark wnd(NULL);
    if(argc > 1) {
        wnd.openMarkdownFile(gApp().StartPath);
        wnd.show();
    } else {
        wnd.show();
    }

    return app.exec();
}
