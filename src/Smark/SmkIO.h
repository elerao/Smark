#ifndef SMARK_SMK_IO_H
#define SMARK_SMK_IO_H

#include <QString>
#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"

/** ****************************************************************************
 * @brief qSmkRemoveFile
 * @param in path 将要删除的文件的路径
 * @return 成功删除文件返回 SMK_EMPTY_STR, 否则返回错误信息
 * ****************************************************************************/
inline QString qSmkRemoveFile(const QString& path) {
    QFile file(path);
    if(false == file.remove())
        return file.errorString();
    else
        return SMK_EMPTY_STR;
}

/** ****************************************************************************
 * @brief qSmkLoadText
 * @param in path 将要读入的文件路径
 * @param out pContext 传出读入文件的内容，以 UTF8 编码
 * @return 成功删除文件返回 SMK_EMPTY_STR, 否则返回错误信息
 * ****************************************************************************/
inline QString qSmkLoadText(const QString& path, QString* pContext) {
    QFile file(path);
    if(false == file.open(QIODevice::ReadOnly)) {
        *pContext = SMK_EMPTY_STR;
        return QString("Load file [" + path + "] error : " + file.errorString());
    } else {
        *pContext = QObject::trUtf8(file.readAll().data());
        return SMK_EMPTY_STR;
    }
}

/** ****************************************************************************
 * @brief qSmkSaveText
 * @param in path 将要保存到的文件路径
 * @param in context 将要写入文件的内容，以 UTF8 来编码
 * @return 成功删除文件返回 SMK_EMPTY_STR, 否则返回错误信息
 * ****************************************************************************/
inline QString qSmkSaveText(const QString& path, const QString& context) {
    QFile file(path);
    if(false == file.open(QIODevice::WriteOnly)) {
        return QString("Save file [" + path + "] error !\n" + file.errorString());
    } else {
        file.write(context.toUtf8());
        return SMK_EMPTY_STR;
    }
}

/** ****************************************************************************
 * @brief qHtmlCachePath
 * @param currentPath 当前 html 文件的路径
 * @return 对应缓存文件的路径
 * ****************************************************************************/
inline QString qHtmlCachePath(const QString& currentPath = SMK_EMPTY_STR) {
#ifdef SMK_WINDOWS_PLATFORM
    const QString cacheDir = qSmkApp()->applicationDirPath();
#else
    const QString cacheDir = "/tmp";
#endif
    return QString("%1/~smark-%2.html.cache")
            .arg(cacheDir)
            .arg(qHash(currentPath));
}

/** * **************************************************************************
 * @brief qMarkCachePath
 * @param currentPath 当前 markdown 文件的路径
 * @return 对应缓存文件的路径
 * ****************************************************************************/
inline QString qMarkCachePath(const QString& currentPath = SMK_EMPTY_STR) {
#ifdef SMK_WINDOWS_PLATFORM
    const QString cacheDir = qSmkApp()->applicationDirPath();
#else
    const QString cacheDir = "/tmp";
#endif
    return QString("%1/~smark-%2.md.cache")
            .arg(cacheDir)
            .arg(qHash(currentPath));
}

#endif // SMARK_SMK_IO_H
