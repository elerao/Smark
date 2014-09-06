#include "Smark/SmkConfig.h"
#include "Smark/SmkMarkWidget.h"

SmkMarkWidget::SmkMarkWidget(QWidget* parent) :
    SmkBasicMarkWidget(parent)
{
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            this,                SLOT(when_VerticalScrollBar_valueChanged(int)) );
}

/* *****************************************************************************
 *
 *                          synchronized scrolling
 *
 * ****************************************************************************/

float SmkMarkWidget::scrollRatio() {
    QScrollBar* scroll = verticalScrollBar();
    return float(scroll->value()-scroll->minimum())
            / float(scroll->maximum()-scroll->minimum());
}

void SmkMarkWidget::when_VerticalScrollBar_valueChanged(int val) {
    QScrollBar* scroll = verticalScrollBar();
    float ratio = float(val-scroll->minimum())
                  / float(scroll->maximum()-scroll->minimum());
    emit verticalScroll(ratio);
}

/* *****************************************************************************
 *
 *                              find and replace
 *
 * ****************************************************************************/

void SmkMarkWidget::findPrevious(const QString& text, bool cased, bool whole) {
    QTextDocument::FindFlags find_flags;
    find_flags |= QTextDocument::FindBackward;
    if(cased)
        find_flags |= QTextDocument::FindCaseSensitively;
    if(whole)
        find_flags |= QTextDocument::FindWholeWords;
    this->find(text, find_flags);
}

void SmkMarkWidget::findNext(const QString& text, bool cased, bool whole) {
    QTextDocument::FindFlags find_flags;
    if(cased)
        find_flags |= QTextDocument::FindCaseSensitively;
    if(whole)
        find_flags |= QTextDocument::FindWholeWords;
    this->find(text, find_flags);
}

void SmkMarkWidget::replaceCurrent(const QString& new_text) {
    QTextCursor cursor = this->textCursor();
    cursor.insertText(new_text);
}

void SmkMarkWidget::replaceAll(const QString& old_text,
                               const QString& new_text,
                               bool cased, bool /*whole*/)
{
    // \todo replace all like this can't redo & undo
    Qt::CaseSensitivity cs = cased ? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString context = this->toPlainText();
    context.replace(old_text, new_text, cs);
    this->setPlainText(context);
}

/* *****************************************************************************
 *
 *                              set selected format
 *
 * ****************************************************************************/

void SmkMarkWidget::setSelectedFormat(const QString& before, const QString& after) {
    QTextCursor cursor = this->textCursor();
    QString   selected = cursor.selectedText();
    if(selected.startsWith(before) && selected.endsWith(after)) {
        selected.remove(before);
        selected.remove(after);
    } else {
        selected.prepend(before);
        selected.append(after);
    }
    cursor.insertText(selected);
}

void SmkMarkWidget::setSelectedBlod(void) {
    setSelectedFormat("<b>", "</b>");
}

void SmkMarkWidget::setSelectedItalic(void) {
    setSelectedFormat("<i>", "</i>");
}

void SmkMarkWidget::setSelectedUnderLine(void) {
    setSelectedFormat("<u>", "</u>");
}

void SmkMarkWidget::setSelectedDeleted(void) {
    setSelectedFormat("<del>", "</del>");
}

void SmkMarkWidget::setSelectedQuote(void) {
    QTextCursor cursor = this->textCursor();
    if(cursor.hasSelection()) {
        QString  selected = cursor.selectedText();
        QStringList lines = selected.split(SMK_LINE_END);
        if(lines.size() == 1) {
            setSelectedFormat("<q>", "</q>");
        } else {
            for(QStringList::iterator it=lines.begin(); it!=lines.end(); ++it)
                it->prepend("> ");
            cursor.insertText(lines.join(SMK_LINE_END));
        }
    } // if(cursor.hasSelection())
}

void SmkMarkWidget::setSelectedCode(void) {
    setSelectedFormat("`", "`");
}

void SmkMarkWidget::setSelectedSub(void) {
    setSelectedFormat("<sub>", "</sub>");
}

void SmkMarkWidget::setSelectedSup(void) {
    setSelectedFormat("<sup>", "</sup>");
}

void SmkMarkWidget::setSelectedBigger(void) {
    setSelectedFormat("<big>", "</big>");
}

void SmkMarkWidget::setSelectedSmaller(void) {
    setSelectedFormat("<small>", "</small>");
}

void SmkMarkWidget::setSelectedOrderedList(void) {
    QTextCursor cursor = this->textCursor();
    QString   selected = cursor.selectedText();
    QStringList  lines = selected.split(SMK_LINE_END);
    int list_num = 1;
    for(QStringList::iterator it=lines.begin(); it!=lines.end(); ++it) {
        if(! it->isEmpty()) {
            it->prepend(" " + QString::number(list_num) + ". ");
            ++list_num;
        }
    }
    cursor.insertText(lines.join(SMK_LINE_END));
}

void SmkMarkWidget::setSelectedUnorderedList(void) {
    QTextCursor cursor = this->textCursor();
    QString selected =  cursor.selectedText();
    QStringList lines = selected.split(SMK_LINE_END);
    for(QStringList::iterator it=lines.begin(); it!=lines.end(); ++it)
        if(! it->isEmpty()) it->prepend(" + ");
    cursor.insertText(lines.join(SMK_LINE_END));
}

void SmkMarkWidget::setSelectedLeftAlign(void) {
    setSelectedFormat("<p align=left>", "</p>");
}

void SmkMarkWidget::setSelectedRightAlign(void) {
    setSelectedFormat("<p align=right>", "</p>");
}

void SmkMarkWidget::setSelectedCenterAlign(void) {
    setSelectedFormat("<center>", "</center>");
}

/* *****************************************************************************
 *
 *                        insert context template
 *
 * ****************************************************************************/

void SmkMarkWidget::insertImage(void) {
    QTextCursor cursor = this->textCursor();
    cursor.insertText("<p align=\"center\">\n"
                      "    <img src=\"____\" width=\"60%\">\n"
                      "    <small> tittle </small>\n"
                      "</p>");
}

void SmkMarkWidget::insertLink(void) {
    QTextCursor cursor = this->textCursor();
    QString selected   = cursor.selectedText();
    if(selected.isEmpty()) {
        cursor.insertText("[___](___)");
    } else {
        selected.prepend("[");
        selected.append("](____)");
        cursor.insertText(selected);
    }
}

void SmkMarkWidget::insertMathJax(void) {
    QTextCursor cursor = this->textCursor();
    QString   selected = cursor.selectedText();
    if(cursor.atBlockStart()) {
        // the cursor is at the start of a block
        // insert a center align math formation
        if(selected.isEmpty()) {
            cursor.insertText("<center>$$\n"
                              "_____\n"
                              "$$</center>");
        } else {
            selected.prepend("<center>$$\n");
            selected.append("$$</center>");
            cursor.insertText(selected);
        }
    } else {
        // the cursor is not at the start of a block
        // insert a inline math formation
        if(selected.isEmpty()) {
            cursor.insertText("${ ____ }$");
        } else {
            selected.prepend("${ ");
            selected.append(" }$");
            cursor.insertText(selected);
        }
    }//if(cursor.atBlockStart()...
}

/* *****************************************************************************
 *
 *                              event porcess
 *
 * ****************************************************************************/

void SmkMarkWidget::keyPressEvent(QKeyEvent* event)
{
    // switch tab char to spaces
    if(event->key() == Qt::Key_Tab) {
        QTextCursor cursor = this->textCursor();
        if(cursor.hasSelection()) {
            QString  selected = cursor.selectedText();
            QStringList lines = selected.split(SMK_LINE_END);
            bool inverse_operation = (event->modifiers() & Qt::ControlModifier) ||
                                     (event->modifiers() & Qt::ShiftModifier);
            if( inverse_operation ) {
                // [Ctrl + Tab] or [Shift + Tab], remove an indentation
                for(QStringList::iterator it=lines.begin(); it!=lines.end(); ++it)
                    if(it->startsWith("    ")) it->remove(0, 4);
            } else {
                // Only Tab, add an indentation
                for(QStringList::iterator it=lines.begin(); it!=lines.end(); ++it)
                    if(! it->isEmpty()) it->prepend("    ");
            }
            cursor.insertText(lines.join(SMK_LINE_END));
        } else { // if(cursor.hasSelection())
            // Just insert a '\t', but we replace '\t' as "    "
            cursor.insertText("    ");
        } // if(cursor.hasSelection())
        return;
    }
    SmkBasicMarkWidget::keyPressEvent(event);
}
