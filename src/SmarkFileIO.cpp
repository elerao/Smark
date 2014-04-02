#include <QDebug>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "SmarkApp.h"
#include "SmarkFileIO.h"

bool gLoadTextFile(const QString& path, QString* context, QString* error) {
    QFile file(path);
    if(file.open(QIODevice::ReadOnly) == false) {
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
    if(file.open(QIODevice::WriteOnly) == false) {
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
    QString path = gApp().CacheDir.path();
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
    QString path = gApp().CacheDir.path();
    if(md_path.isEmpty()) {
        path += "/~smark-000001.cache.html";
    } else {
        path += "/~smark-";
        path += QString::number(qHash(md_path));
        path += ".cache.html";
    }
    return path;
}

SmarkConfig gLoadConfigFile(const QString &path)
{
    // pre-build file key-value map
    QMap<QString, QString> kvmap;
    kvmap["gui.menu"]    = "show";
    kvmap["gui.toolbar"] = "show";
    kvmap["gui.mode"]    = "read";

    // read config file as a key-value map
    QFile file(path);
    if(file.open(QIODevice::ReadOnly) == true) {
        QTextStream stream(&file);
        QStringList list;
        while(! stream.atEnd()) {
            list = stream.readLine().remove(" ").remove("\t").split("=");
            kvmap[ list[0] ] = list[1];
        }
    }

    // parse config key-value map
    SmarkConfig config;
    config.ShowMenu    = ("show" == kvmap["gui.menu"]);
    config.ShowToolbar = ("show" == kvmap["gui.toolbar"]);
    if("read" == kvmap["gui.mode"])
        config.StartMode = ReadMode;
    else if("edit" == kvmap["gui.mode"])
        config.StartMode = EditMode;
    else
        config.StartMode = PreviewMode;
    return config;
}

bool gSaveConfigFile(const QString &path, const SmarkConfig& config)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly) == false)
        return false;

    QTextStream stream(&file);
    stream << "gui.menu    = " << ((config.ShowMenu==true) ? "show" : "hide") << endl;
    stream << "gui.toolbar = " << ((config.ShowToolbar==true) ? "show" : "hide") << endl;
    stream << "gui.mode    = ";
    if(config.StartMode == EditMode)
        stream << "edit\n";
    else if(config.StartMode == ReadMode)
        stream << "read\n";
    else
        stream << "preview\n";
    stream.flush();
    return true;
}
