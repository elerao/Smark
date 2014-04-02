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

    void parseMarkToHtml(const QString& markPath,
                         const QString& htmlPath,
                         const QString& cssPath);

    void parse(const QString& inPath,  const QString& inFormat,
               const QString& outPath, const QString& outFormat,
               const QString& cssPath);

signals:

    void completed(void);

private slots:

    void when_mark_to_html_finished(void);
    void when_parser_finished(void);
};

#endif // SMARK_PARSER_H
