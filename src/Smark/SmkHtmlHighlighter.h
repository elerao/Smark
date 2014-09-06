#ifndef SMARK_SMK_HTML_HIGHLIGHTER_H
#define SMARK_SMK_HTML_HIGHLIGHTER_H

#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include "Smark/SmkConfig.h"

class SmkHtmlHighlighter
        : public QSyntaxHighlighter
{
//    Q_OBJECT

private:

    QHash<int, QColor> colors_;

public:

    explicit SmkHtmlHighlighter(QTextDocument* document);

protected:

    virtual void highlightBlock(const QString& text);
};

#endif // SMARK_SMK_HTML_HIGHLIGHTER_H
