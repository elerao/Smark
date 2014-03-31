#ifndef GUI_HTML_VIEW_H
#define GUI_HTML_VIEW_H

#include <QWebView>

class GuiHtmlView
        : public QWebView
{
    Q_OBJECT

public:

    explicit GuiHtmlView(QWidget *parent = 0);

public: // clear html context

    void clearHtml(void);

public: // synchronized scrolling

    int scrollValue(void);
    void setScrollValue(int value);

public slots:

    void setScrollRatio(float ratio);

};

#endif // GUI_HTML_VIEW_H
