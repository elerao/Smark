#ifndef SMARK_SMK_MARK_PARSER_H
#define SMARK_SMK_MARK_PARSER_H

#include <QObject>
#include "Smark/SmkConfig.h"

QT_BEGIN_NAMESPACE
class QString;
class QThread;
QT_END_NAMESPACE

/** ****************************************************************************
 * @brief The SmkMarkParser class
 * Smark 程序所使用的 Markdown 解析器类
 * 目前的实现是在另一线程中基于 QProcess 后台调用 Pandoc 命令来完成
 * ****************************************************************************/
class SmkMarkParser : public QObject
{
    Q_OBJECT

private:

    QThread* parserThread_;

public:

    explicit SmkMarkParser(QObject *parent = 0);
    virtual ~SmkMarkParser(void);

    //! 将路径为 markPath 的 markdown 文件转换为
    //! 路径为 htmlPath 的 html 格式文件
    //! 使用路径为 cssPath 的 stylesheet 文件
    bool parseMarkToHtml(const QString& markPath,
                         const QString& htmlPath,
                         const QString& cssPath );

    //! 将路径为 markPath 的 inFormat 格式文件转换为
    //! 路径为 htmlPath 的 outFormat 格式文件
    //! 使用路径为 cssPath 的 stylesheet 文件
    bool parse(const QString& inPath,  const QString& inFormat,
               const QString& outPath, const QString& outFormat,
               const QString& cssPath );

signals:

    //! 当转换操作完成时发射的信号
    void completed(void);

private slots:

    //! 当 markdown 向 html 的转换完成时将调用的槽
    void when_mark_to_html_finished(void);

    //! 当转换操作完成时将调用的槽
    void when_parser_finished(void);
};

#endif // SMARK_SMK_MARK_PARSER_H
