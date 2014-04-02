#include <QStringList>
#include <QProcess>
#include <QThread>
#include "SmarkParser.h"

/* *****************************************************************************
 *
 *                              Pandoc Thread
 *
 * ****************************************************************************/

class PandocThread
        : public QThread
{
private:

    QStringList _args;

public:

    PandocThread(const QString& markPath,
                 const QString& htmlPath,
                 const QString& cssPath ) {
        _args << markPath
              << "--from" << "markdown"
              << "--to"   << "html"
              << "--css"  << cssPath
              << "--mathjax"
              << "--highlight-style" << "haddock"
              << "-o"     << htmlPath;
    }

    PandocThread(const QString& inPath,  const QString& inFormat,
                 const QString& outPath, const QString& outFormat,
                 const QString& cssPath) {
        _args << inPath
              << "--from" << inFormat
              << "--to"   << outFormat
              << "--css"  << cssPath
              << "--mathjax"
              << "--highlight-style" << "haddock"
              << "-o"     << outPath;
    }

protected:

    virtual void run() {
        QString program("pandoc");
        if(_args.isEmpty())
            QProcess::execute(program);
        else
            QProcess::execute(program, _args);
    }
};

/* *****************************************************************************
 *
 *                              Smark Parser
 *
 * ****************************************************************************/

SmarkParser::SmarkParser(QObject *parent)
    : QObject(parent), _parser_thread(NULL) {
    /* nothign need to do */
}
SmarkParser::~SmarkParser(void) {
    if(_parser_thread != NULL)
        delete _parser_thread;
}

void SmarkParser::parseMarkToHtml(const QString& markPath,
                                  const QString& htmlPath,
                                  const QString& cssPath) {
    if(_parser_thread == NULL) {
        _parser_thread = new PandocThread(markPath, htmlPath, cssPath);
        connect(_parser_thread, SIGNAL(finished()),
                this, SLOT(when_mark_to_html_finished()) );
        _parser_thread->start();
    }
}

void SmarkParser::parse(const QString& inPath,  const QString& inFormat,
                        const QString& outPath, const QString& outFormat,
                        const QString& cssPath) {
    if(_parser_thread == NULL) {
        _parser_thread = new PandocThread(inPath,  inFormat,
                                          outPath, outFormat,
                                          cssPath);
        connect(_parser_thread, SIGNAL(finished()),
                this, SLOT(when_parser_finished()) );
        _parser_thread->start();
    }
}

void SmarkParser::when_mark_to_html_finished(void ) {
    if(_parser_thread != NULL) {
        disconnect(_parser_thread, SIGNAL(finished()),
                   this, SLOT(when_mark_to_html_finished()) );
        delete _parser_thread;
        emit completed();
    }
    _parser_thread = NULL;
}

void SmarkParser::when_parser_finished(void ) {
    if(_parser_thread != NULL) {
        disconnect(_parser_thread, SIGNAL(finished()),
                   this, SLOT(when_parser_finished()) );
        delete _parser_thread;
    }
    _parser_thread = NULL;
}

