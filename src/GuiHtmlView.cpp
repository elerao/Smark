#include <QTextDocument>
#include <QtWebKit/QWebFrame>
#include "SmarkFileIO.h"
#include "SmarkApp.h"
#include "GuiHtmlView.h"

GuiHtmlView::GuiHtmlView(QWidget* parent)
    : QWebView(parent)
{

}

void GuiHtmlView::clearHtml(void) {
    setHtml(QString());
    update();
}

/* *****************************************************************************
 *
 *                          synchronized scrolling
 *
 * ****************************************************************************/

int GuiHtmlView::scrollValue(void) {
    return page()->mainFrame()->scrollBarValue(Qt::Vertical);
}

void GuiHtmlView::setScrollValue(int value) {
    page()->mainFrame()->setScrollBarValue(Qt::Vertical, value);
}

void GuiHtmlView::setScrollRatio(float ratio) {
    int maxv = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    int minv = page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
    page()->mainFrame()->setScrollBarValue(Qt::Vertical,
                                           minv + ratio*(maxv-minv));
}

