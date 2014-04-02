#ifndef GUI_SMARK_MAIN_WINDOW_H
#define GUI_SMARK_MAIN_WINDOW_H

#include <QMainWindow>
#include "SmarkParser.h"
#include "SmarkApp.h"

namespace Ui {
    class GuiWindowSmark;
}

class GuiWindowSmark : public QMainWindow
{
    Q_OBJECT

private: // aux function

    void _aux_connectSignalAndSlot(void);
    void _aux_switchDisplayMode(SmarkMode mode);
    void _aux_initGUI(void);
    void _aux_enableGUI(void);
    void _aux_disableGUI(void);
    void _aux_setCurrentPath(const QString& path);
    bool _aux_cancelCurrentOperation(void);
    void _aux_parseMarkdownToHTML(void);

public: // ctor & dtor

    explicit GuiWindowSmark(QWidget *parent = 0);
    ~GuiWindowSmark(void);

public: // open a markdown file

    void openMarkdownFile(const QString& path);

protected: // event process

    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void closeEvent(QCloseEvent *);

private slots: // GUI slots

    // menu file
    void on_ActionFileNew_triggered(void);
    void on_ActionFileOpen_triggered(void);
    void on_ActionFileSave_triggered(void);
    void on_ActionFileSaveAs_triggered(void);
    void on_ActionFileClose_triggered(void);
    void on_ActionFileQuite_triggered(void);
    void on_ActionFileExport_triggered(void);
    void on_ActionFileExportHTML_triggered(void);
    void on_ActionFileExportDocx_triggered(void);
    void on_ActionFileExportLaTeX_triggered(void);
    void on_ActionFileExportOpenOfficeODT_triggered(void);
    void on_ActionFileExportReStructuredText_triggered(void);
    void on_ActionFileExportMediaWikiMarkup_triggered(void);
    void on_ActionFileExportEPUB_triggered(void);
    void on_ActionFileExportPlainText_triggered(void);


    // menu view
    void on_ActionViewReadOnly_triggered(void);
    void on_ActionViewEditOnly_triggered(void);
    void on_ActionViewPreview_triggered(void);
    void on_ActionViewFullScreen_triggered(void);
    void on_ActionViewHideMenu_triggered(void);
    void on_ActionViewHideToolbar_triggered(void);

    // menu edit
    void on_ActionEditFlush_triggered(void);
    void on_ActionEditCut_triggered(void);
    void on_ActionEditCopy_triggered(void);
    void on_ActionEditPaste_triggered(void);
    void on_ActionEditRedo_triggered(void);
    void on_ActionEditUndo_triggered(void);
    void on_ActionEditFind_triggered(void);
    void on_ActionEditCSS_triggered(void);
    void on_ActionEditCopyHTML_triggered();

    // menu insert
    void on_ActionInsertImage_triggered(void);
    void on_ActionInsertLink_triggered(void);
    void on_ActionInsertFormation_triggered(void);

    // menu format
    void on_ActionFormatBlod_triggered(void);
    void on_ActionFormatItalic_triggered(void);
    void on_ActionFormatUnderline_triggered(void);
    void on_ActionFormatBigger_triggered(void);
    void on_ActionFormatSmaller_triggered(void);
    void on_ActionFormatSub_triggered(void);
    void on_ActionFormatSup_triggered(void);
    void on_ActionFormatCode_triggered(void);
    void on_ActionFormatQuote_triggered(void);
    void on_ActionFormatAlignLeft_triggered();
    void on_ActionFormatAlignRight_triggered();
    void on_ActionFormatAlignCenter_triggered();

    // menu help
    void on_ActionHelpAboutQt_triggered(void);
    void on_ActionHelpAboutSmark_triggered(void);

    // widget find
    void on_FindPrevButton_clicked(void);
    void on_FindNextButton_clicked(void);
    void on_ReplaceButton_clicked(void);
    void on_ReplaceAllButton_clicked(void);

    // inner slot
    void when_parser_process_finish(void);
    void when_mark_is_edit(void);
    void when_css_is_edit(void);

private: // private member data

    Ui::GuiWindowSmark *ui;
    SmarkMode   _current_mode;
    SmarkParser _parser;

    bool    _is_modified;
    bool    _CSS_is_modified;
    QString _current_path;
    QString _mark_cache_path;
    QString _html_cache_path;
    QString _html;
};

#endif // GUI_SMARK_MAIN_WINDOW_H
