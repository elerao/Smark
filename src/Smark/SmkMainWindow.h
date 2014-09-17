#ifndef SMARK_SMK_MAINWINDOW_H
#define SMARK_SMK_MAINWINDOW_H

#include <QMainWindow>
#include "Smark/SmkConfig.h"
#include "Smark/SmkMarkParser.h"
#include "Smark/SmkHtmlHighlighter.h"

namespace Ui {
    class SmkMainWindow;
}

enum SmkGuiMode {
    eSmkEditMode    = 0,
    eSmkReadMode    = 1,
    eSmkPreviewMode = 2
};

class SmkMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit SmkMainWindow(QWidget *parent = 0);
    ~SmkMainWindow();

public: // open a markdown file

    void openMarkdownFile(const QString& path);

private: // aux function

    void _aux_connectSignalAndSlot(void);
    void _aux_switchDisplayMode(SmkGuiMode mode);
    void _aux_initGUI(void);
    void _aux_enableGUI(void);
    void _aux_disableGUI(void);
    void _aux_setCurrentPath(const QString& path);
    bool _aux_cancelCurrentOperation(void);
    void _aux_parseMarkdownToHTML(void);

protected: // event process

    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void closeEvent(QCloseEvent *);

private slots:

    // 文件菜单
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionClose_triggered();
    void on_actionOption_triggered();
    void on_actionPrint_triggered();
    void on_actionQuite_triggered();

    // 视图菜单
    void on_actionPreview_triggered();
    void on_actionReadOnly_triggered();
    void on_actionEditOnly_triggered();
    void on_actionFullScreen_triggered(bool checked);

    // 编辑菜单
    void on_actionFlush_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_actionFind_triggered();
    void on_actionViewHTML_triggered();

    // 插入菜单
    void on_actionInsertImage_triggered();
    void on_actionInsertLink_triggered();
    void on_actionInsertMathJax_triggered();

    // 格式菜单
    void on_actionFormatBlod_triggered();
    void on_actionFormatItalic_triggered();
    void on_actionFormatUnderline_triggered();
    void on_actionFormatBigger_triggered();
    void on_actionFormatSmaller_triggered();
    void on_actionFormatSub_triggered();
    void on_actionFormatSup_triggered();
    void on_actionFormatCode_triggered();
    void on_actionFormatQuote_triggered();
    void on_actionAlignLeft_triggered();
    void on_actionAlignRight_triggered();
    void on_actionAlignCenter_triggered();

    // 帮助菜单
    void on_actionAboutQt_triggered();
    void on_actionAboutSmark_triggered();

    // 查找部件
    void on_buttonFindPrev_clicked();
    void on_buttonFindNext_clicked();
    void on_buttonReplace_clicked();
    void on_buttonReplaceAll_clicked();

    // 光标移动时同步两个视图
    void on_markView_cursorPositionChanged();

    // 后台调用 Pandoc 相关
    void when_parser_process_finish(void);
    void when_mark_is_edit(void);

private:

    Ui::SmkMainWindow* ui;
    QPlainTextEdit*     pHtmlEditor_;
    SmkHtmlHighlighter* pHtmlHighlighter_;

    bool          isModified_;
    SmkMarkParser parser_;
    SmkGuiMode    currentGuiMode_;
    QString       currentPath_;
    QString       markCachePath_;
    QString       htmlCachePath_;
    QString       htmlContext_;
};

#endif // SMARK_SMK_MAINWINDOW_H
