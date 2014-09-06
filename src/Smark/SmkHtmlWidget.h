#ifndef SMARK_SMK_HTML_WIDGET_H
#define SMARK_SMK_HTML_WIDGET_H

#include <QWebView>

class SmkHtmlWidget
        : public QWebView
{
    Q_OBJECT

public:

    explicit SmkHtmlWidget(QWidget *parent = 0);

/* *****************************************************************************
 *
 *                       get context and clear context
 *
 * ****************************************************************************/

public:

    // clear html context
    void clearHtml(void);

/* *****************************************************************************
 *
 *                          synchronized scrolling
 *
 * ****************************************************************************/

public:

    // synchronized scrolling
    int scrollValue(void);
    void setScrollValue(int value);

public slots:

    void setScrollRatio(float ratio);

};

#endif // SMARK_SMK_HTML_WIDGET_H
