#ifndef SMARK_FILE_IO_H
#define SMARK_FILE_IO_H

#include <QString>
#include <QFileInfo>
#include "SmarkApp.h"

inline QString gFileDir(const QString& file_path) {
    QFileInfo info(file_path);
    return info.path();
}

inline QString gFileName(const QString& file_path) {
    QFileInfo info(file_path);
    return info.fileName();
}

inline QString gFileSuffix(const QString& file_path) {
    QFileInfo info(file_path);
    return info.suffix().toLower();
}

inline QString gDefaultPath(const QString& path, const QString& suffix) {
    QFileInfo info(path);
    return (info.path() + "/" + info.completeBaseName() + suffix);
}

QString gHtmlCachePath(const QString& md_path = QString());

QString gMarkCachePath(const QString& md_path = QString());

bool gRemoveFile(const QString &path);

bool gLoadTextFile(const QString& path, QString* context, QString* error = NULL);

bool gSaveTextFile(const QString& path, const QString& context, QString* error = NULL);

SmarkConfig gLoadConfigFile(const QString &path);

bool gSaveConfigFile(const QString &path, const SmarkConfig& config);


#endif // SMARK_FILE_IO_H
