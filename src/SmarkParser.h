#ifndef SMARK_PARSER_H
#define SMARK_PARSER_H

#include <QObject>

class QString;
class QThread;

class SmarkParser
        : public QObject
{
     Q_OBJECT

private:

    QThread* _parser_thread;

public:

    SmarkParser(QObject *parent = 0);
    virtual ~SmarkParser(void);
    void parseMarkToHtml(const QString& mark_path, const QString& html_path);

signals:

    void completed(void);

private slots:

    void when_parser_finished(void);
};

#endif // SMARK_PARSER_H
