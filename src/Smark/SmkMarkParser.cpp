#include <QProcess>
#include <QThread>
#include "Smark/SmkConfig.h"
#include "Smark/SmkMarkParser.h"

namespace SmkPrivate {
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
}//namespace SmkPrivate

SmkMarkParser::SmkMarkParser(QObject *parent)
    : QObject(parent), parserThread_(NULL) {
    /* nothign need to do */
}

SmkMarkParser::~SmkMarkParser(void) {
    if(parserThread_ != NULL)
        delete parserThread_;
}

//! 将路径为 markPath 的 markdown 文件转换为
//! 路径为 htmlPath 的 html 格式文件
//! 使用路径为 cssPath 的 stylesheet 文件
void SmkMarkParser::parseMarkToHtml(const QString& markPath,
                                    const QString& htmlPath,
                                    const QString& cssPath ) {
    if(parserThread_ == NULL) {
        parserThread_ = new SmkPrivate::PandocThread(markPath, htmlPath, cssPath);
        connect(parserThread_, SIGNAL(finished()),
                this,           SLOT(when_mark_to_html_finished()) );
        parserThread_->start();
    }
}

//! 将路径为 markPath 的 inFormat 格式文件转换为
//! 路径为 htmlPath 的 outFormat 格式文件
//! 使用路径为 cssPath 的 stylesheet 文件
void SmkMarkParser::parse(const QString& inPath,  const QString& inFormat,
                          const QString& outPath, const QString& outFormat,
                          const QString& cssPath ) {
    if(parserThread_ == NULL) {
        parserThread_ = new SmkPrivate::PandocThread(inPath,  inFormat,
                                                     outPath, outFormat,
                                                     cssPath);
        connect(parserThread_, SIGNAL(finished()),
                this,           SLOT(when_parser_finished()) );
        parserThread_->start();
    }
}

//! 当 markdown 向 html 的转换完成时将调用的槽
void SmkMarkParser::when_mark_to_html_finished(void) {
    if(parserThread_ != NULL) {
        disconnect(parserThread_, SIGNAL(finished()),
                   this,           SLOT(when_mark_to_html_finished()) );
        delete parserThread_;
        emit completed();
    }
    parserThread_ = NULL;
}

//! 当转换操作完成时将调用的槽
void SmkMarkParser::when_parser_finished(void) {
    if(parserThread_ != NULL) {
        disconnect(parserThread_, SIGNAL(finished()),
                   this,           SLOT(when_parser_finished()) );
        delete parserThread_;
    }
    parserThread_ = NULL;
}
