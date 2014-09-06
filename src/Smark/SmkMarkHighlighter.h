#ifndef SMARK_SMK_MARK_HIGHLIGHTER_H
#define SMARK_SMK_MARK_HIGHLIGHTER_H

#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include "Smark/SmkConfig.h"

class SmkMarkHighlighter
        : public QSyntaxHighlighter
{
//    Q_OBJECT

private:

    QVector<QRegExp>         regexp_;
    QVector<QTextCharFormat> format_;

private:

    void _aux_multiBlockMatch(
            const QString&         text,
            const QString&         bgnFlag,
            const QString&         endFlag,
            const QTextCharFormat& format,
            const unsigned         field );

public:

    explicit SmkMarkHighlighter(QTextDocument* parent = 0);

public:

    void updateOption(void);

protected:

    virtual void highlightBlock(const QString& text);
};

#endif // SMARK_SMK_MARK_HIGHLIGHTER_H
