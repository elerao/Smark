#ifndef SMARK_SMK_APP_H
#define SMARK_SMK_APP_H

#include "Smark/SmkConfig.h"

/** ****************************************************************************
 * @brief The SmkApp class
 * Smark 应用程序类，继承自 QApplication，使用单例模式
 * 新添功能：
 *   + Smark 应用程序配置文件的读写
 *   + Smark 应用程序的全局选项
 *
 * ****************************************************************************/
class SmkApp : public QApplication
{
    Q_OBJECT

private: // private data member

    QHash<QString, QString> options_;

public: // ctor & dtor

    SmkApp(int & argc, char** argv);
    virtual ~SmkApp(void);

public: // option information

    //! 重设为默认设置
    void loadDefaultOption(void);

    //! 获取指定 key 的设置
    QString option(const QString& key) const;

    //! 设置指定 key 的设置值
    void setOption(const QString& key, const QString& val);

    //! 保存当前设置
    void saveOption(void) const;
};

/** ****************************************************************************
 * @brief qSmkApp
 * @return 返回 SmkApp 的单例指针
 *
 * ****************************************************************************/
inline SmkApp* qSmkApp(void) {
    return static_cast<SmkApp*>(QCoreApplication::instance());
}

#endif // SMARK_SMK_APP_H
