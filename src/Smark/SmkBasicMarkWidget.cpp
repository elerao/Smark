#include <QtGui>
#include "Smark/SmkApp.h"
#include "Smark/SmkBasicMarkWidget.h"

namespace SmkPrivate {
    class LineNumberArea : public QWidget
    {
    private:

        SmkBasicMarkWidget* editor;

    public:

        LineNumberArea(SmkBasicMarkWidget* parent)
            : QWidget(parent), editor(parent) {
            /*nothing need to do*/
        }

        QSize sizeHint() const {
            return QSize(editor->lineNumberAreaWidth(), 0);
        }

    protected:

        void paintEvent(QPaintEvent *event) {
            editor->lineNumberAreaPaintEvent(event);
        }
    };
}//namespace SmkPrivate

SmkBasicMarkWidget::SmkBasicMarkWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    pLineNumberArea_ = new SmkPrivate::LineNumberArea(this);
    pHighlighter_    = new SmkMarkHighlighter(this->document());

    connect(this, SIGNAL(blockCountChanged(int)),
            this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(const QRect&, int)),
            this, SLOT(updateLineNumberArea(const QRect&, int)));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    updateOption();
}

void SmkBasicMarkWidget::updateOption(void)
{
    // set editor font
    QFont font;
    font.setFamily(qSmkApp()->option("font.family"));
    font.setPointSize(qSmkApp()->option("font.size").toInt());
    font.setStyleHint(QFont::Monospace, QFont::PreferAntialias);
    setFont(font);

    // set edit color
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(qSmkApp()->option("color.background")));
    palette.setColor(QPalette::Text, QColor(qSmkApp()->option("color.foreground")));
    setPalette(palette);

    // set highter option
    pHighlighter_->updateOption();
    pHighlighter_->rehighlight();
}

int SmkBasicMarkWidget::lineNumberAreaWidth(void) {
    int digits = 1;
    int maxi   = qMax(1, blockCount());
    while(maxi >= 10) {
        maxi /= 10;
        ++digits;
    }
    return (8 + fontMetrics().width(QLatin1Char('9')) * digits);
}

void SmkBasicMarkWidget::updateLineNumberAreaWidth(int /*newBlockCount*/) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void SmkBasicMarkWidget::updateLineNumberArea(const QRect &rect, int dy) {
    if(dy != 0)
        pLineNumberArea_->scroll(0, dy);
    else
        pLineNumberArea_->update(0, rect.y(), pLineNumberArea_->width(), rect.height());
    if(rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void SmkBasicMarkWidget::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect contentsRect = this->contentsRect();
    pLineNumberArea_->setGeometry(QRect(contentsRect.left(),   contentsRect.top(),
                                      lineNumberAreaWidth(), contentsRect.height()));
}

void SmkBasicMarkWidget::highlightCurrentLine()
{
    if(isReadOnly()) return;
    QTextCursor currentCursor = textCursor();

    QTextEdit::ExtraSelection lineSelection;
    lineSelection.format.setBackground( QColor(qSmkApp()->option("color.focusline")) );
    lineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineSelection.cursor = currentCursor;
    lineSelection.cursor.clearSelection();

    QTextEdit::ExtraSelection wordSelection;
    wordSelection.format.setBackground( QColor(qSmkApp()->option("color.focusword")) );
    wordSelection.cursor = currentCursor;
    wordSelection.cursor.select(QTextCursor::WordUnderCursor);

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(lineSelection);
    extraSelections.append(wordSelection);
    setExtraSelections(extraSelections);
}

void SmkBasicMarkWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QColor numberBkgColor, numberFkgColor;
    numberBkgColor = QColor(qSmkApp()->option("color.background"));
    int gray = qGray(numberBkgColor.red(), numberBkgColor.green(), numberBkgColor.blue());
    if(gray < 64) {
        // dark background
        numberBkgColor = numberBkgColor.lighter(300);
        numberFkgColor = Qt::gray;
    } else {
        // light background
        numberBkgColor = Qt::lightGray;
        numberFkgColor = Qt::darkGray;
    }

    QPainter painter(pLineNumberArea_);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.fillRect(event->rect(), numberBkgColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while(block.isValid() && (top <= event->rect().bottom()) )
    {
        if(block.isVisible() && (bottom >= event->rect().top()) ) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(numberFkgColor);
            painter.drawText(0, top,
                             pLineNumberArea_->width()-5, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block  = block.next();
        top    = bottom;
        bottom = top + int(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
