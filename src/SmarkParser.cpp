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

    PandocThread(const QString& mark_path, const QString& html_path ) {
        _args << "-f" << "markdown" << "-t" << "html"
             << "-i" << mark_path
             << "-o" << html_path;
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

void SmarkParser::parseMarkToHtml(const QString& mark_path,
                                  const QString& html_path) {
    if(_parser_thread == NULL) {
        _parser_thread = new PandocThread(mark_path, html_path);
        connect(_parser_thread, SIGNAL(finished()),
                this, SLOT(when_parser_finished()) );
        _parser_thread->start();
    }
}

void SmarkParser::when_parser_finished(void ) {
    if(_parser_thread != NULL) {
        disconnect(_parser_thread, SIGNAL(finished()),
                   this, SLOT(when_parser_finished()) );
        delete _parser_thread;
        emit completed();
    }
    _parser_thread = NULL;
}

