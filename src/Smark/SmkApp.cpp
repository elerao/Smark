#include <QDataStream>
#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"

SmkApp::SmkApp(int & argc, char** argv)
    : QApplication(argc, argv)
{
    // 不管配置文件不存在，都先载入默认设置
    // 这样一来即便新添了配置项也同样兼容原先的配置文件
    loadDefaultOption();

    // 而后检查是否存在配置文件
    // 若配置文件存在，载入配置文件中的设置
    // the config file is there, load the options in the option file
    QHash<QString, QString> options;
    QString optionPath = qSmkApp()->applicationDirPath() + "/smark.qconfig";
    QFile   optionFile(optionPath);
    if(optionFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&optionFile);
        stream >> options;
    }
    typedef QHash<QString, QString>::const_iterator hash_iterator;
    for(hash_iterator it=options.begin(); it!=options.end(); it++)
        options_[it.key()] = it.value();
    setStyle(options_["gui.style"]);
}

SmkApp::~SmkApp(void) {
    this->saveOption();
}

//! 重设为默认设置
void SmkApp::loadDefaultOption(void)
{
    options_["gui.style"]        = QStyleFactory::keys().last();
    options_["gui.toolbar"]      = "TRUE";

    options_["font.size"]        = "12";
#ifdef SMK_WINDOWS_PLATFORM
    options_["font.family"]      = trUtf8("Consolas,华文细黑");
#else
    options_["font.family"]      = trUtf8("Monospace,等宽微米黑");
#endif

    options_["color.background"] = "#000408";
    options_["color.foreground"] = "#fff0f0";
    options_["color.focusline"]  = "#08080f";
    options_["color.focusword"]  = "#181820";
    options_["color.header"]     = "#ffcc00";
    options_["color.code"]       = "#00ccff";
    options_["color.latex"]      = "#88ccff";
    options_["color.html"]       = "#88cc00";
    options_["color.link"]       = "#00ffcc";

#ifdef SMK_WINDOWS_PLATFORM
    // 部分 Windows 系统下 Qt WebView 中 MathJAx 公式字体过小
    options_["text.head"]   = "<style type=\"text/css\">"
                              "\n  span.MathJax_SVG { zoom : 1.6; }"
                              "\n</style>";
#else
    options_["text.head"] = "";
#endif
    options_["text.hat"]    = "";
    options_["text.tail"]   = "";
    options_["text.foot"]   = "";

    // 如果本地的 MathJax.js 存在就不使用 http://cdn.mathjax.org 的 MathJax.js
    QString localMathJaxUrl = qSmkApp()->applicationDirPath() + "/mathjax/MathJax.js";
    if(QFileInfo(localMathJaxUrl).exists())
        options_["url.mathjax"] = localMathJaxUrl;
    else
        options_["url.mathjax"] = "http://cdn.mathjax.org/mathjax/latest/MathJax.js";

    // 如果本地的 smark.css 不存在就使用我博客上的在线 css，目前二者是一样的
    QString localCSSUrl = qSmkApp()->applicationDirPath() + "/smark.css";
    if(QFileInfo(localCSSUrl).exists())
        options_["url.css"] = localCSSUrl;
    else
        options_["url.css"] = "http://elerao.github.io/css/page.css";
}

//! 获取指定 key 的设置
QString SmkApp::option(const QString& key) const {
    return options_.value(key);
}

//! 设置指定 key 的设置值
void SmkApp::setOption(const QString& key, const QString& val) {
    options_[key] = val;
}

//! 保存当前设置
void SmkApp::saveOption(void) const {
    QString optionPath = qSmkApp()->applicationDirPath() + "/smark.qconfig";
    QFile   optionFile(optionPath);
    if(false == optionFile.open(QIODevice::WriteOnly)) {
        qDebug() << "[ERROR] Fail to open " << optionPath << " for writing !";
    } else {
        QDataStream stream(&optionFile);
        stream << options_;
    }
}
