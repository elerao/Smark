#include <QHash>
#include <QFile>
#include <QFileInfo>
#include "SmarkApp.h"
#include "SmarkFileIO.h"

bool gLoadTextFile(const QString& path, QString* context, QString* error) {
    QFile file(path);
    if(! file.open(QIODevice::ReadOnly) ) {
        if(error != NULL)
            *error = "Load file [" + path + "] error !\n" + file.errorString();
        return false;
    }
    *context = QObject::trUtf8(file.readAll().data());
    file.close();
    return true;
}

bool gSaveTextFile(const QString& path, const QString& context, QString* error) {
    QFile file(path);
    if(! file.open(QIODevice::WriteOnly)) {
        if(error != NULL)
            *error = "Save file [" + path + "] error !\n" + file.errorString();
        return false;
    }
    file.write(context.toUtf8());
    file.close();
    return true;
}

bool gRemoveFile(const QString &path) {
    QFile file(path);
    return file.remove();
}

QString gHtmlCachePath(const QString& md_path) {
    QString path = gApp().BinDir.path();
    if(md_path.isEmpty()) {
        path += "/~smark-000001.cache.md";
    } else {
        path += "/~smark-";
        path += QString::number(qHash(md_path));
        path += ".cache.md";
    }
    return path;
}

QString gMarkCachePath(const QString& md_path) {
    QString path = gApp().BinDir.path();
    if(md_path.isEmpty()) {
        path += "/~smark-000001.cache.html";
    } else {
        path += "/~smark-";
        path += QString::number(qHash(md_path));
        path += ".cache.html";
    }
    return path;
}
