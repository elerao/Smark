#include <QTextDocument>
#include "Smark/SmkConfig.h"
#include "Smark/SmkHtmlWidget.h"

#if (QT_VERSION < 0x050000)
#   include <QtWebKit/QWebFrame>
#endif

SmkHtmlWidget::SmkHtmlWidget(QWidget* parent)
    : QWebView(parent) {
}

/* *****************************************************************************
 *
 *                       get context and clear context
 *
 * ****************************************************************************/

void SmkHtmlWidget::clearHtml(void) {
    setHtml(SMK_EMPTY_STR);
    update();
}

/* *****************************************************************************
 *
 *                          synchronized scrolling
 *
 * ****************************************************************************/

int SmkHtmlWidget::scrollValue(void) {
    return page()->mainFrame()->scrollBarValue(Qt::Vertical);
}

void SmkHtmlWidget::setScrollValue(int value) {
    page()->mainFrame()->setScrollBarValue(Qt::Vertical, value);
}

void SmkHtmlWidget::setScrollRatio(float ratio) {
    int maxv = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    int minv = page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
    page()->mainFrame()->setScrollBarValue(Qt::Vertical,
                                           minv + ratio*(maxv-minv));
}
