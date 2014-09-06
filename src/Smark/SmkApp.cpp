#include <QDataStream>
#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"

SmkApp::SmkApp(int & argc, char** argv)
    : QApplication(argc, argv)
{
    QString optionPath = qSmkApp()->applicationDirPath() + "/smark.qconfig";
    QFile   optionFile(optionPath);
    if(false == optionFile.open(QIODevice::ReadOnly)) {
        // 配置文件不存在，载入默认设置
        // the config file is not existed, load the default options
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

        options_["text.head"]   = "";
        options_["text.hat"]    = "";
        options_["text.tail"]   = "";
        options_["text.foot"]   = "";

        options_["url.mathjax"] = "http://cdn.mathjax.org/mathjax/latest/MathJax.js";
        options_["url.css"]     = qSmkApp()->applicationDirPath() + "/smark.css";
    } else {
        // 配置文件存在，载入配置文件中的设置
        // the config file is there, load the options in the option file
        QDataStream stream(&optionFile);
        stream >> options_;
    }
    qDebug() << "Style : " << QStyleFactory::keys();
    setStyle(options_["gui.style"]);
}

SmkApp::~SmkApp(void) {
    this->saveOption();
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
