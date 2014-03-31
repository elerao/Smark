#ifndef SMARK_FILE_IO_H
#define SMARK_FILE_IO_H

#include <QString>
#include <QFileInfo>

inline QString gFileDir(const QString& file_path) {
    QFileInfo info(file_path);
    return info.path();
}

inline QString gFileName(const QString& file_path) {
    QFileInfo info(file_path);
    return info.fileName();
}

bool gRemoveFile(const QString &path);

bool gLoadTextFile(const QString& path, QString* context, QString* error = NULL);

bool gSaveTextFile(const QString& path, const QString& context, QString* error = NULL);

QString gHtmlCachePath(const QString& md_path);
QString gMarkCachePath(const QString& md_path);

#endif // SMARK_FILE_IO_H
