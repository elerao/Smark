#ifndef SMARK_SMK_CONFIG_H
#define SMARK_SMK_CONFIG_H

#include <QString>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#if (QT_VERSION >= 0x050000)
#include <QtWidgets>
#include <QtWebKitWidgets>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#endif

#define SMK_VERSION "smark 2.0"

#ifdef _WIN32
# define SMK_WINDOWS_PLATFORM
#endif

const QChar   SMK_LINE_END(L'\u2029');
const QString SMK_EMPTY_STR("");
#define SMK_EMPTY_STR SMK_EMPTY_STR
#define SMK_LINE_END  SMK_LINE_END

#endif // SMARK_SMK_CONFIG_H
