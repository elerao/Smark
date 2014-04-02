#include <QtGui>
#include <QtWebKit>
#include "SmarkFileIO.h"
#include "GuiWindowSmark.h"
#include "ui_GuiWindowSmark.h"

/* *****************************************************************************
 *
 *                                 ctor & dtor
 *
 * ****************************************************************************/

 GuiWindowSmark::GuiWindowSmark(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GuiWindowSmark),
      _is_modified(false),
      _CSS_is_modified(false),
      _mark_cache_path(gMarkCachePath()),
      _html_cache_path(gHtmlCachePath())
{
    ui->setupUi(this);
    _aux_initGUI();
    _aux_disableGUI();
    _aux_connectSignalAndSlot();
    // _mark_cache_path = gMarkCachePath(_current_path);
    // _html_cache_path = gHtmlCachePath(_current_path);
}

GuiWindowSmark::~GuiWindowSmark(void) {
    delete ui;
}

/* *****************************************************************************
 *
 *                              aux function
 *
 * ****************************************************************************/

void GuiWindowSmark::_aux_connectSignalAndSlot(void)
{
    connect(ui->EditMark, SIGNAL(textChanged()),
            this,         SLOT(when_mark_is_edit()) );
    connect(ui->EditCSS, SIGNAL(textChanged()),
            this,        SLOT(when_css_is_edit()) );
    connect(&_parser, SIGNAL(completed()),
            this,     SLOT(when_parser_process_finish()) );
    connect(ui->EditMark, SIGNAL(verticalScroll(float)),
            ui->WebView,  SLOT(setScrollRatio(float)) );
}

void GuiWindowSmark::_aux_switchDisplayMode(SmarkMode mode)
{
    switch(mode) {
    case EditMode :
        ui->WidgetHTML->hide();
        ui->WidgetMark->show();
        if(ui->ToolBarNormal->isVisible())
            ui->ToolBarEditor->show();
        ui->EditMark->setFocus();
        break;
    case ReadMode :
        ui->WidgetMark->hide();
        ui->ToolBarEditor->hide();
        ui->WidgetHTML->show();
        ui->WebView->setFocus();
        ui->SplitterHTML->setOrientation(Qt::Vertical);
        break;
    case PreviewMode :
    default:
        ui->WidgetMark->show();
        ui->WidgetHTML->show();
        if(ui->ToolBarNormal->isVisible())
            ui->ToolBarEditor->show();
        ui->EditMark->setFocus();
        if(height() > 0.73*width()) {
            QList<int> list;
            list.append(ui->CentralWidget->height()/2);
            list.append(ui->CentralWidget->height()/2);
            ui->SplitterMain->setOrientation(Qt::Vertical);
            ui->SplitterMain->setSizes(list);
            ui->SplitterHTML->setOrientation(Qt::Horizontal);
        } else {
            QList<int> list;
            list.append(ui->CentralWidget->width()/2);
            list.append(ui->CentralWidget->width()/2);
            ui->SplitterMain->setOrientation(Qt::Horizontal);
            ui->SplitterMain->setSizes(list);
            ui->SplitterHTML->setOrientation(Qt::Vertical);
        }
        break;
    }
    _current_mode = mode;
}

void GuiWindowSmark::_aux_initGUI(void)
{
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    ui->EditMark->setPalette(palette);
    ui->EditCSS->setPalette(palette);

    // set Editor font
    QFont font;
    font.setFamily("Consolas");
    font.setPointSize(12);
    ui->EditMark->setFont(font);
    ui->EditCSS->setFont(font);

    // set CSS test
    ui->EditCSS->setPlainText(gApp().CSS);

    // hide widgets until we need them
    _aux_switchDisplayMode(gApp().Config.StartMode);
    ui->EditCSS->hide();
    ui->FindGroup->hide();
    if(false == gApp().Config.ShowMenu) {
        ui->MenuBar->hide();
        ui->ActionViewHideMenu->setText("Show Menu");
    }
    if(false == gApp().Config.ShowToolbar) {
        ui->ToolBarNormal->hide();
        ui->ToolBarEditor->hide();
        ui->ActionViewHideToolbar->setText("Show Toolbar");
    }

    // set drops accept
    ui->WebView->setAcceptDrops(false);
    ui->EditMark->setAcceptDrops(false);
    setAcceptDrops(true);
}

void GuiWindowSmark::_aux_enableGUI(void)
{
    // center widget
    ui->CentralWidget->setEnabled(true);

    // file menu
    ui->ActionFileSave->setEnabled(true);
    ui->ActionFileSaveAs->setEnabled(true);
    ui->ActionFileClose->setEnabled(true);
    ui->ActionFileExport->setEnabled(true);
    ui->MenuExportAs->setEnabled(true);

    // edit menu
    ui->ActionEditFlush->setEnabled(true);
    ui->ActionEditCut->setEnabled(true);
    ui->ActionEditCopy->setEnabled(true);
    ui->ActionEditPaste->setEnabled(true);
    ui->ActionEditRedo->setEnabled(true);
    ui->ActionEditUndo->setEnabled(true);
    ui->ActionEditFind->setEnabled(true);
    ui->ActionEditCSS->setEnabled(true);
    ui->ActionEditCopyHTML->setEnabled(true);

    // insert menu
    ui->ActionInsertImage->setEnabled(true);
    ui->ActionInsertLink->setEnabled(true);
    ui->ActionInsertFormation->setEnabled(true);

    // format menu
    ui->ActionFormatBlod->setEnabled(true);
    ui->ActionFormatItalic->setEnabled(true);
    ui->ActionFormatUnderline->setEnabled(true);
    ui->ActionFormatBigger->setEnabled(true);
    ui->ActionFormatSmaller->setEnabled(true);
    ui->ActionFormatSub->setEnabled(true);
    ui->ActionFormatSup->setEnabled(true);
    ui->ActionFormatCode->setEnabled(true);
    ui->ActionFormatQuote->setEnabled(true);
    ui->ActionFormatAlignCenter->setEnabled(true);
    ui->ActionFormatAlignLeft->setEnabled(true);
    ui->ActionFormatAlignRight->setEnabled(true);
}

void GuiWindowSmark::_aux_disableGUI(void)
{
    // center widget
    ui->CentralWidget->setEnabled(false);

    // file menu
    ui->ActionFileSave->setEnabled(false);
    ui->ActionFileSaveAs->setEnabled(false);
    ui->ActionFileClose->setEnabled(false);
    ui->ActionFileExport->setEnabled(false);
    ui->MenuExportAs->setEnabled(false);

    // edit menu
    ui->ActionEditFlush->setEnabled(false);
    ui->ActionEditCut->setEnabled(false);
    ui->ActionEditCopy->setEnabled(false);
    ui->ActionEditPaste->setEnabled(false);
    ui->ActionEditRedo->setEnabled(false);
    ui->ActionEditUndo->setEnabled(false);
    ui->ActionEditFind->setEnabled(false);
    ui->ActionEditCSS->setEnabled(false);
    ui->ActionEditCopyHTML->setEnabled(false);

    // insert menu
    ui->ActionInsertImage->setEnabled(false);
    ui->ActionInsertLink->setEnabled(false);
    ui->ActionInsertFormation->setEnabled(false);

    // format menu
    ui->ActionFormatBlod->setEnabled(false);
    ui->ActionFormatItalic->setEnabled(false);
    ui->ActionFormatUnderline->setEnabled(false);
    ui->ActionFormatBigger->setEnabled(false);
    ui->ActionFormatSmaller->setEnabled(false);
    ui->ActionFormatSub->setEnabled(false);
    ui->ActionFormatSup->setEnabled(false);
    ui->ActionFormatCode->setEnabled(false);
    ui->ActionFormatQuote->setEnabled(false);
    ui->ActionFormatAlignCenter->setEnabled(false);
    ui->ActionFormatAlignLeft->setEnabled(false);
    ui->ActionFormatAlignRight->setEnabled(false);
}

void GuiWindowSmark::_aux_setCurrentPath(const QString& path)
{
    _current_path = path;
    _mark_cache_path   = gMarkCachePath(path);
    _html_cache_path   = gHtmlCachePath(path);
    if(path.isEmpty()) {
        setWindowTitle("Smark");
    } else {
        QString wnd_title =  "Smark - " + ::gFileName(path);
        setWindowTitle(wnd_title);
    }
}

bool GuiWindowSmark::_aux_cancelCurrentOperation(void)
{
    const bool Cancel = true;
    const bool   GoOn = false;
    if(_is_modified == false) return GoOn;

    // show a dialog to let user chose
    QMessageBox::StandardButton select
            = QMessageBox::warning(
                  this,
                  tr("Warning"),
                  tr("Do you want to save your"
                     " modifications to the current document ?"),
                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                  QMessageBox::Cancel );

    if(QMessageBox::Cancel == select) {
        return Cancel;
    } else if(QMessageBox::Yes == select) {
        // if current path is empty, select a path to save
        if(_current_path.isEmpty()) {
            QString path = QFileDialog::getSaveFileName(
                               this,
                               tr("Save Current Markdown File"),
                               tr("~/") );
            if(path.isEmpty()) return Cancel;
            _aux_setCurrentPath(path);
        }
        // save current markdown context
        QString error, context;
        context = ui->EditMark->toPlainText();
        if(false == gSaveTextFile(_current_path, context, &error)) {
            QMessageBox::warning(this, tr("Error"), error);
            return Cancel;
        } else {
            _is_modified = false;
            return GoOn;
        }
    }// if...else if...
    return GoOn;
}

void GuiWindowSmark::_aux_parseMarkdownToHTML(void)
{
    QString error;
    QString text = ui->EditMark->toPlainText();
    if(false == gSaveTextFile(_mark_cache_path, text, &error))
        QMessageBox::warning(this, "Write Cache File Error", error);
    else
        _parser.parseMarkToHtml(_mark_cache_path, _html_cache_path, gApp().CSSPath);
}

/* *****************************************************************************
 *
 *                             open a markdown file
 *
 * ****************************************************************************/

void GuiWindowSmark::openMarkdownFile(const QString& path) {
    QString error, context;
    if(false == gLoadTextFile(path, &context, &error)) {
        QMessageBox::warning(this, tr("Error"), error);
        _aux_disableGUI();
    } else {
        _aux_enableGUI();
        ui->EditMark->setPlainText(context);
        _is_modified = false;
        _aux_setCurrentPath(path);
        on_ActionEditFlush_triggered();
    }
}

/* *****************************************************************************
 *
 *                              event process
 *
 * ****************************************************************************/

void GuiWindowSmark::resizeEvent(QResizeEvent* event) {
    // do not call QMainWindow::resizeEvent(event);
    QMainWindow::resizeEvent(event);
    _aux_switchDisplayMode(_current_mode);
}

void GuiWindowSmark::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape) {
        // Omnipotent Esc Key
        if(ui->FindGroup->isVisible()) {
            ui->FindGroup->hide();
            ui->EditMark->setFocus();
        } else if(ui->EditCSS->isVisible()) {
            ui->EditCSS->hide();
            ui->WebView->setFocus();
        } else if(ui->WidgetMark->isVisible()) {
            _aux_switchDisplayMode(ReadMode);
        } else if(ui->MenuBar->isVisible()
                  || ui->ToolBarNormal->isVisible()
                  || ui->ToolBarEditor->isVisible() ) {
            ui->MenuBar->hide();
            ui->ToolBarNormal->hide();
            ui->ToolBarEditor->hide();
        } else if(isFullScreen()) {
            showNormal();
        } else {
            on_ActionFileQuite_triggered();
        }
    }

    // if the menu & the actions is hide,
    // we have to mapped the short key here
    if(false == menuBar()->isVisible())
    {
        if(event->modifiers() & Qt::AltModifier) {
            // hotkey Alt + ?
            // show the menu widget
            menuWidget()->show();
        } else if(event->modifiers() & Qt::ControlModifier) {
            // hotkey Ctrl + Shift + ?
            if(event->modifiers() & Qt::ShiftModifier) {
                switch(event->key()) {
                case (Qt::Key_E): on_ActionFileExport_triggered();      return;
                case (Qt::Key_S): on_ActionFileSaveAs_triggered();      return;
                case (Qt::Key_C): on_ActionEditCopyHTML_triggered();    return;
                case (Qt::Key_L): on_ActionInsertLink_triggered();      return;
                case (Qt::Key_M): on_ActionInsertFormation_triggered(); return;
                case (Qt::Key_I): on_ActionInsertImage_triggered();     return;
                default: break;
                }
            }

            // hotkey Ctrl + ?
            switch(event->key()) {
            case (Qt::Key_N):            on_ActionFileNew_triggered();         return;
            case (Qt::Key_O):            on_ActionFileOpen_triggered();        return;
            case (Qt::Key_S):            on_ActionFileSave_triggered();        return;
            case (Qt::Key_W):            on_ActionFileClose_triggered();       return;
            case (Qt::Key_Q):            on_ActionFileQuite_triggered();       return;
            case (Qt::Key_C):            on_ActionEditCopy_triggered();        return;
            case (Qt::Key_X):            on_ActionEditCut_triggered();         return;
            case (Qt::Key_P):            on_ActionEditPaste_triggered();       return;
            case (Qt::Key_F):            on_ActionEditFind_triggered();        return;
            case (Qt::Key_Z):            on_ActionEditUndo_triggered();        return;
            case (Qt::Key_Y):            on_ActionEditRedo_triggered();        return;
            case (Qt::Key_G):            on_ActionEditCSS_triggered();         return;
            case (Qt::Key_B):            on_ActionFormatBlod_triggered();      return;
            case (Qt::Key_I):            on_ActionFormatItalic_triggered();    return;
            case (Qt::Key_U):            on_ActionFormatUnderline_triggered(); return;
            case (Qt::Key_QuoteLeft):    on_ActionFormatCode_triggered();      return; // do not work ?
            case (Qt::Key_AsciiTilde):   on_ActionFormatCode_triggered();      return; // do not work ?
            case (Qt::Key_Apostrophe):   on_ActionFormatQuote_triggered();     return; // do not work ?
            case (Qt::Key_Down):         on_ActionFormatSub_triggered();       return;
            case (Qt::Key_Up):           on_ActionFormatSup_triggered();       return;
            case (Qt::Key_BracketRight): on_ActionFormatBigger_triggered();    return;
            case (Qt::Key_BracketLeft):  on_ActionFormatSmaller_triggered();   return;
            case (Qt::Key_L):            on_ActionFormatAlignLeft_triggered(); return;
            case (Qt::Key_R):            on_ActionFormatAlignRight_triggered();  return;
            case (Qt::Key_E):            on_ActionFormatAlignCenter_triggered(); return;
            default: break;
            }
        } else {
            // hotkey F?
            switch(event->key()) {
            case (Qt::Key_F6):  on_ActionViewPreview_triggered();     return;
            case (Qt::Key_F7):  on_ActionViewReadOnly_triggered();    return;
            case (Qt::Key_F8):  on_ActionViewEditOnly_triggered();    return;
            case (Qt::Key_F11): on_ActionViewFullScreen_triggered();  return;
            case (Qt::Key_F12): on_ActionViewHideMenu_triggered();    return;
            case (Qt::Key_F10): on_ActionViewHideToolbar_triggered(); return;
            case (Qt::Key_F5):  on_ActionEditFlush_triggered();       return;
            default: break;
            }
        }
    }//if..else...
    QMainWindow::keyPressEvent(event);
}

void GuiWindowSmark::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        event->accept();
    } else if(event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
        event->accept();
    }
    QMainWindow::dragEnterEvent(event);
}

void GuiWindowSmark::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if(false == urls.empty()) {
        QString path = urls.first().toLocalFile();
        if(false == _aux_cancelCurrentOperation()) {
            openMarkdownFile(path);
        }
    }
    QMainWindow::dropEvent(event);
}

void GuiWindowSmark::closeEvent(QCloseEvent* event) {
    if(false == _aux_cancelCurrentOperation())
        event->accept();
    else
        event->ignore();
}

/* *****************************************************************************
 *
 *                                  Menu File
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionFileNew_triggered(void) {
    if(false == _aux_cancelCurrentOperation()) {
        _is_modified = false;
        _aux_setCurrentPath(QString());
        ui->WebView->clearHtml();
        ui->EditMark->clear();
        _aux_enableGUI();
    }
}

void GuiWindowSmark::on_ActionFileOpen_triggered(void) {
    if(false == _aux_cancelCurrentOperation()) {
        QString path = QFileDialog::getOpenFileName(
                           this,
                           tr("Open File"),
                           ::gFileDir(_current_path),
                           tr("markdown files (*.markdown *.md *mkd);; all files (*.*)"));
        if(path.isEmpty()) return;
        openMarkdownFile(path);
    }
}

void GuiWindowSmark::on_ActionFileSave_triggered(void)
{
    // if current path is empty, select a path to save
    if(_current_path.isEmpty()) {
        QString path = QFileDialog::getSaveFileName(
                           this,
                           tr("Save Current Markdown File"),
                           tr("~/"),
                           tr("markdown files (*.markdown *.md *mkd);; all files (*.*)"));
        if(path.isEmpty()) return;
        _aux_setCurrentPath(path);
    }

    // save current markdown context
    QString error, context;
    context = ui->EditMark->toPlainText();
    if(false == gSaveTextFile(_current_path, context, &error)) {
        QMessageBox::warning(this, tr("Error"), error);
    } else {
        _is_modified = false;
        on_ActionEditFlush_triggered();
    }
}

void GuiWindowSmark::on_ActionFileSaveAs_triggered(void)
{
    // select a path to save
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Save Current Markdown File As"),
                       gFileDir(_current_path),
                       tr("markdown files (*.markdown *.md *mkd);; all files (*.*)"));
    if(path.isEmpty()) return;

    // save current markdown context
    QString error, context;
    context = ui->EditMark->toPlainText();
    if(false == gSaveTextFile(_current_path, context, &error))
        QMessageBox::warning(this, tr("Error"), error);
}

void GuiWindowSmark::on_ActionFileClose_triggered(void) {
    if(false == _aux_cancelCurrentOperation()) {
        ui->EditMark->clear();
        ui->WebView->clearHtml();
        _is_modified = false;
        _aux_setCurrentPath(QString());
        _aux_disableGUI();
    }
}

void GuiWindowSmark::on_ActionFileQuite_triggered(void) {
    if(false == _aux_cancelCurrentOperation()) {
        ui->EditMark->clear();
        ui->WebView->clearHtml();
        _is_modified = false;
        _aux_setCurrentPath(QString());
        _aux_disableGUI();
        qApp->quit();
    }
}

void GuiWindowSmark::on_ActionFileExport_triggered(void)
{
    // get export file path
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As HTML"),
                       gDefaultPath(_current_path, ".html"),
                       tr("HTML files (*.html);;"
                          "Microsoft Word (*.docx);;"
                          "Tex Text (*.tex);;"
                          "OpenOffice ODT (*.odt);;"
                          "reStructured Text (*.rst);;"
                          "Wiki Markup (*.wiki);;"
                          "EPUB ebook (*.epub);;"
                          "Plain Text (*.txt);;"
                          "all files (*.*)") );
    if(path.isEmpty() == false) {
        QString suffix = gFileSuffix(path);
        if(suffix == "html") {
            gSaveTextFile(path, _html);
            return;
        } else {
            QString format;
            if(suffix == "docx")
                format = "docx";
            else if(suffix == "tex")
                format = "latex";
            else if(suffix == "odt")
                format = "odt";
            else if(suffix == "rst")
                format = "rst";
            else if(suffix == "wiki")
                format = "mediawiki";
            else if(suffix == "epub")
                format = "epub";
            else if(suffix == "txt")
                format = "plain";
            else
                format = "plain";
            _parser.parse(_current_path, "markdown", path, format, gApp().CSSPath);
        }
    }//if..else..
}

void GuiWindowSmark::on_ActionFileExportHTML_triggered() {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As HTML"),
                       gDefaultPath(_current_path, ".html"),
                       tr("HTML files (*.html);; all files (*.*)") );
    if(path.isEmpty() == false)
        gSaveTextFile(path, _html);
}

void GuiWindowSmark::on_ActionFileExportDocx_triggered() {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As Microsoft Word"),
                       gDefaultPath(_current_path, ".docx"),
                       tr("Microsoft Word (*.docx);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "docx", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportLaTeX_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As LaTex File"),
                       gDefaultPath(_current_path, ".tex"),
                       tr("Tex Text (*.tex);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "latex", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportOpenOfficeODT_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As OpenOffice ODT"),
                       gDefaultPath(_current_path, ".odt"),
                       tr("OpenOffice ODT (*.odt);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "odt", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportReStructuredText_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As reStructured Text"),
                       gDefaultPath(_current_path, ".rst"),
                       tr("reStructured Text (*.rst);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "rst", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportMediaWikiMarkup_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As Media Wiki Markup"),
                       gDefaultPath(_current_path, ".wiki"),
                       tr("Wiki Markup (*.wiki);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "mediawiki", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportEPUB_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As EPUB"),
                       gDefaultPath(_current_path, ".epub"),
                       tr("EPUB ebook (*.epub);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "epub", gApp().CSSPath);
    }
}

void GuiWindowSmark::on_ActionFileExportPlainText_triggered(void) {
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As Plain Text"),
                       gDefaultPath(_current_path, ".txt"),
                       tr("Text File (*.txt);; all files (*.*)") );
    if(path.isEmpty() == false) {
        _parser.parse(_current_path, "markdown", path, "plain", gApp().CSSPath);
    }
}

/* *****************************************************************************
 *
 *                                  Menu View
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionViewReadOnly_triggered(void) {
    _aux_switchDisplayMode(ReadMode);
}

void GuiWindowSmark::on_ActionViewEditOnly_triggered(void) {
    _aux_switchDisplayMode(EditMode);
}

void GuiWindowSmark::on_ActionViewPreview_triggered(void) {
    _aux_switchDisplayMode(PreviewMode);
}

void GuiWindowSmark::on_ActionViewFullScreen_triggered(void) {
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}

void GuiWindowSmark::on_ActionViewHideMenu_triggered(void) {
    if(menuWidget()->isVisible()) {
        menuWidget()->hide();
        gApp().Config.ShowMenu = false;
    } else {
        menuWidget()->show();
        gApp().Config.ShowMenu = true;
    }
}

void GuiWindowSmark::on_ActionViewHideToolbar_triggered(void) {
    if(ui->ToolBarNormal->isVisible() || ui->ToolBarEditor->isVisible()) {
        ui->ToolBarNormal->hide();
        ui->ToolBarEditor->hide();
        gApp().Config.ShowToolbar = false;
    } else {
        ui->ToolBarNormal->show();
        if(ui->EditMark->isVisible())
            ui->ToolBarEditor->show();
        gApp().Config.ShowToolbar = true;
    }
}

/* *****************************************************************************
 *
 *                                  Menu Edit
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionEditFlush_triggered(void) {
    // save the CSS file if need at first
    if(_CSS_is_modified) {
        gApp().CSS = ui->EditCSS->toPlainText();
        ::gSaveTextFile(gApp().CSSPath, gApp().CSS);
        _CSS_is_modified = false;
        // we must clear the setting of Web View
        // otherwise the new css will not works
        ui->WebView->settings()->clearMemoryCaches();
    }
    _aux_parseMarkdownToHTML();
}

void GuiWindowSmark::on_ActionEditCut_triggered(void) {
    QWidget* focus = focusWidget();
    if(focus == ui->EditMark) {
        ui->EditMark->cut();
    } else if(focus == ui->EditCSS) {
        ui->EditCSS->cut();
    }
}

void GuiWindowSmark::on_ActionEditCopy_triggered(void) {
    QWidget* focus = focusWidget();
    if(focus == ui->EditMark) {
        ui->EditMark->copy();
    } else if(focus == ui->EditCSS) {
        ui->EditCSS->copy();
    } else if(focus == ui->WebView) {
        QString      selected = ui->WebView->selectedText();
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(selected);
    }
}

void GuiWindowSmark::on_ActionEditPaste_triggered(void) {
    QWidget* focus = focusWidget();
    if(focus == ui->EditMark) {
        // todo insert a image or link in clipboard
        ui->EditMark->paste();
    } else if(focus == ui->EditCSS) {
        ui->EditCSS->paste();
    }
}

void GuiWindowSmark::on_ActionEditRedo_triggered(void) {
    QWidget* focus = focusWidget();
    if(focus == ui->EditMark) {
        ui->EditMark->redo();
    } else if(focus == ui->EditCSS) {
        ui->EditCSS->redo();
    }
}

void GuiWindowSmark::on_ActionEditUndo_triggered(void) {
    QWidget* focus = focusWidget();
    if(focus == ui->EditMark) {
        ui->EditMark->undo();
    } else if(focus == ui->EditCSS) {
        ui->EditCSS->undo();
    }
}

void GuiWindowSmark::on_ActionEditFind_triggered(void) {
    if(ui->WidgetMark->isVisible()) {
        if(ui->FindGroup->isVisible()) {
            ui->FindGroup->hide();
        } else {
            QString selected = ui->EditMark->textCursor().selectedText();
            ui->FindLineEdit->setText(selected);
            ui->FindNextButton->setFocus();
            ui->FindGroup->show();
        }
    }
}

void GuiWindowSmark::on_ActionEditCSS_triggered(void) {
    if(ui->WidgetHTML->isVisible()) {
        if(ui->EditCSS->isVisible()) {
            ui->EditCSS->hide();
        } else {
            ui->EditCSS->setPlainText(gApp().CSS);
            ui->EditCSS->show();
        }
    }
}

void GuiWindowSmark::on_ActionEditCopyHTML_triggered() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(_html);
}


/* *****************************************************************************
 *
 *                                  Menu Insert
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionInsertImage_triggered(void) {
    if(focusWidget() == ui->EditMark)
        ui->EditMark->insertImage();
}

void GuiWindowSmark::on_ActionInsertLink_triggered(void) {
    if(focusWidget() == ui->EditMark)
        ui->EditMark->insertLink();
}

void GuiWindowSmark::on_ActionInsertFormation_triggered(void) {
    if(focusWidget() == ui->EditMark)
        ui->EditMark->insertFormation();
}

/* *****************************************************************************
 *
 *                                  Menu Format
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionFormatBlod_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedBlod();
    }
}

void GuiWindowSmark::on_ActionFormatItalic_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedItalic();
    }
}

void GuiWindowSmark::on_ActionFormatUnderline_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedUnderLine();
    }
}

void GuiWindowSmark::on_ActionFormatBigger_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedBigger();
    }
}

void GuiWindowSmark::on_ActionFormatSmaller_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSmaller();
    }
}

void GuiWindowSmark::on_ActionFormatSub_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSub();
    }
}

void GuiWindowSmark::on_ActionFormatSup_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSup();
    }
}

void GuiWindowSmark::on_ActionFormatCode_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedCode();
    }
}

void GuiWindowSmark::on_ActionFormatQuote_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedQuote();
    }
}

void GuiWindowSmark::on_ActionFormatAlignLeft_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedLeftAlign();
    }
}

void GuiWindowSmark::on_ActionFormatAlignRight_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedRightAlign();
    }
}

void GuiWindowSmark::on_ActionFormatAlignCenter_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedCenterAlign();
    }
}

/* *****************************************************************************
 *
 *                                  Menu About
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionHelpAboutQt_triggered(void) {
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void GuiWindowSmark::on_ActionHelpAboutSmark_triggered(void) {
    QString about_smark = tr(
                              "<h1>Smark Editor 1.1</h1>"
                              "A markdown editor bases on Qt4 & pandoc under GPL"
                              "<br />"
                              "Copyright @ <a href=\"elerao.ao@gmail.com\">elerao.ao@gmail.com</a> 2013~2014" );
    QMessageBox::about(this, tr("About Smark"), about_smark);
}

/* *****************************************************************************
 *
 *                                 Find Widget
 *
 * ****************************************************************************/

void GuiWindowSmark::on_FindPrevButton_clicked(void) {
    ui->EditMark->findPrevious(ui->FindLineEdit->text(),
                               ui->CaseCheckBox->isChecked(),
                               ui->WholeWordCheckBox->isChecked());
}

void GuiWindowSmark::on_FindNextButton_clicked(void) {
    ui->EditMark->findNext(ui->FindLineEdit->text(),
                           ui->CaseCheckBox->isChecked(),
                           ui->WholeWordCheckBox->isChecked());

}

void GuiWindowSmark::on_ReplaceButton_clicked(void) {
    ui->EditMark->replaceCurrent(ui->ReplaceLineEdit->text());
}

void GuiWindowSmark::on_ReplaceAllButton_clicked(void) {
    ui->EditMark->replaceAll(ui->FindLineEdit->text(),
                             ui->ReplaceLineEdit->text(),
                             ui->CaseCheckBox->isChecked(),
                             ui->WholeWordCheckBox->isChecked() );
}

/* *****************************************************************************
 *
 *                               inner slot
 *
 * ****************************************************************************/

void GuiWindowSmark::when_parser_process_finish(void)
{
    QString context, error;
    if(false == gLoadTextFile(_html_cache_path, &context, &error)) {
        QMessageBox::warning(this, tr("Error"), error);
    } else {
        // update webview, do not use update()
        _html.swap(context);
        int position = ui->WebView->scrollValue();
        ui->WebView->setHtml(_html, QUrl::fromLocalFile(_current_path));
        ui->WebView->repaint(ui->WebView->page()->mainFrame()->geometry());
        ui->WebView->setScrollValue(position);

        // remove cache file
        gRemoveFile(_html_cache_path);
        gRemoveFile(_mark_cache_path);
    }
}

void GuiWindowSmark::when_mark_is_edit(void) {
    _is_modified = true;
}
void GuiWindowSmark::when_css_is_edit(void) {
    _CSS_is_modified = true;
}


