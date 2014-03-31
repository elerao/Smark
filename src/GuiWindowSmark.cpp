#include <QtGui>
#include <QtWebKit>
#include "SmarkFileIO.h"
#include "GuiWindowSmark.h"
#include "ui_GuiWindowSmark.h"

/* *****************************************************************************
 *
 *                              aux function
 *
 * ****************************************************************************/

void GuiWindowSmark::_aux_signal_slot_connect(void)
{
    connect(ui->EditMark, SIGNAL(textChanged()),
            this, SLOT(when_mark_is_edit()) );
    connect(ui->EditCSS, SIGNAL(textChanged()),
            this, SLOT(when_css_is_edit()) );
    connect(&_parser, SIGNAL(completed()),
            this, SLOT(when_parser_process_finish()) );
    connect(ui->EditMark, SIGNAL(verticalScroll(float)),
            ui->WebView,  SLOT(setScrollRatio(float)) );
}

void GuiWindowSmark::_aux_switch_GUI_mode(SmarkMode mode)
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
    gApp().CurrentMode = mode;
}

void GuiWindowSmark::_aux_init_GUI(void)
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

    // hide widgets until we need them
    ui->EditCSS->hide();
    ui->FindGroup->hide();
    menuWidget()->hide();
    // ui->ToolBarNormal->hide();
    // ui->ToolBarEditor->hide();
}

void GuiWindowSmark::_aux_enable_GUI(void)
{
    // center widget
    ui->CentralWidget->setEnabled(true);

    // file menu
    ui->ActionFileSave->setEnabled(true);
    ui->ActionFileSaveAs->setEnabled(true);
    ui->ActionFileClose->setEnabled(true);
    ui->ActionFileExport->setEnabled(true);

    // edit menu
    ui->ActionEditFlush->setEnabled(true);
    ui->ActionEditCut->setEnabled(true);
    ui->ActionEditCopy->setEnabled(true);
    ui->ActionEditPaste->setEnabled(true);
    ui->ActionEditRedo->setEnabled(true);
    ui->ActionEditUndo->setEnabled(true);
    ui->ActionEditFind->setEnabled(true);
    ui->ActionEditCSS->setEnabled(true);

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
}

void GuiWindowSmark::_aux_disable_GUI(void)
{
    // center widget
    ui->CentralWidget->setEnabled(false);

    // file menu
    ui->ActionFileSave->setEnabled(false);
    ui->ActionFileSaveAs->setEnabled(false);
    ui->ActionFileClose->setEnabled(false);
    ui->ActionFileExport->setEnabled(false);

    // edit menu
    ui->ActionEditFlush->setEnabled(false);
    ui->ActionEditCut->setEnabled(false);
    ui->ActionEditCopy->setEnabled(false);
    ui->ActionEditPaste->setEnabled(false);
    ui->ActionEditRedo->setEnabled(false);
    ui->ActionEditUndo->setEnabled(false);
    ui->ActionEditFind->setEnabled(false);
    ui->ActionEditCSS->setEnabled(false);

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
}

void GuiWindowSmark::_aux_set_current_path(const QString& path)
{
    _current_path = path;
    _mark_cache   = gMarkCachePath(path);
    _html_cache   = gHtmlCachePath(path);
    if(path.isEmpty()) {
        setWindowTitle("Smark");
    } else {
        QString wnd_title =  "Smark - " + ::gFileName(path);
        setWindowTitle(wnd_title);
    }
}

bool GuiWindowSmark::_aux_cancel_current_operation(void)
{
    const bool Cancel = true, GoOn = false;
    if(_is_modified) {
        QMessageBox::StandardButton select = QMessageBox::warning(
                                                 this,
                                                 tr("Warning"),
                                                 tr("Do you want to save your"
                                                    " modifications to the current document ?"),
                                                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                 QMessageBox::Cancel);

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
                _aux_set_current_path(path);
            }
            // save current markdown context
            QString error, context;
            context = ui->EditMark->toPlainText();
            if(! gSaveTextFile(_current_path, context, &error)) {
                QMessageBox::warning(this, tr("Error"), error);
                return Cancel;
            } else {
                _is_modified = false;
                return GoOn;
            }
        }// if...else if...
    }//if(_is_modified)
    return GoOn;
}

void GuiWindowSmark::_aux_do_parse(void)
{
    QString error, text;
    text = ui->EditMark->toPlainText();
    if(! gSaveTextFile(_mark_cache, text, &error)) {
        QMessageBox::warning(this, "Write Cache File Error", error);
    } else {
        _parser.parseMarkToHtml(_mark_cache, _html_cache);
    }
}

/* *****************************************************************************
 *
 *                                 ctor & dtor
 *
 * ****************************************************************************/

GuiWindowSmark::GuiWindowSmark(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::GuiWindowSmark),
      _is_modified(false), _CSS_is_modified(false)
{
    ui->setupUi(this);
    _aux_init_GUI();
    _aux_disable_GUI();
    _aux_signal_slot_connect();
    _aux_switch_GUI_mode(gApp().CurrentMode);
    _mark_cache = gMarkCachePath(_current_path);
    _html_cache = gHtmlCachePath(_current_path);
}

GuiWindowSmark::~GuiWindowSmark(void) {
    if(_CSS_is_modified) { // save modified CSS to file
        gApp().CSS = ui->EditCSS->toPlainText();
        ::gSaveTextFile(gApp().CSSPath, gApp().CSS);
    }
    delete ui;
}

/* *****************************************************************************
 *
 *                             open a markdown file
 *
 * ****************************************************************************/

void GuiWindowSmark::openMarkdownFile(const QString& path) {
    QString error, context;
    if(! gLoadTextFile(path, &context, &error)) {
        QMessageBox::warning(this, tr("Error"), error);
        _aux_disable_GUI();
    } else {
        _aux_enable_GUI();
        ui->EditMark->setPlainText(context);
        _is_modified = false;
        _aux_set_current_path(path);
        on_ActionEditFlush_triggered();
    }
}

/* *****************************************************************************
 *
 *                              event process
 *
 * ****************************************************************************/

void GuiWindowSmark::resizeEvent(QResizeEvent *) {
    _aux_switch_GUI_mode(gApp().CurrentMode);

}
void GuiWindowSmark::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape) {
        // Omnipotent Esc Key
        static bool ExitFullScreenNextTime = false;
        if(ExitFullScreenNextTime) {
            if(this->isFullScreen())
                this->showNormal();
            ExitFullScreenNextTime = false;
        }
        if(ui->FindGroup->isVisible()) {
            ui->FindGroup->hide();
            ui->EditMark->setFocus();
            ExitFullScreenNextTime = true;
        } else if(ui->EditCSS->isVisible()) {
            ui->EditCSS->hide();
            ui->WebView->setFocus();
            ExitFullScreenNextTime = true;
        } else if(ui->WidgetMark->isVisible()) {
            _aux_switch_GUI_mode(ReadMode);
            ExitFullScreenNextTime = true;
        }
    }

    if(! menuBar()->isVisible()) {
        // the menu & the actions is hide, so we have to mapped
        // the short key here

        if(event->modifiers() & Qt::ShiftModifier & Qt::ControlModifier) {
            // hotkey Ctrl + Shift + ?
            switch(event->key()) {
            case (Qt::Key_E): on_ActionFileExport_triggered();      return;
            case (Qt::Key_S): on_ActionFileSaveAs_triggered();      return;
            case (Qt::Key_L): on_ActionInsertLink_triggered();      return;
            case (Qt::Key_M): on_ActionInsertFormation_triggered(); return;
            case (Qt::Key_I): on_ActionInsertImage_triggered();     return;
            default: break;
            }
        }
        else if(event->modifiers() & Qt::ControlModifier) {
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
            case (Qt::Key_Alt): on_ActionViewHideMenu_triggered();    return;
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
    QMainWindow::dragEnterEvent(event);
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        event->accept();
    } else if(event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
        event->accept();
    }
}

void GuiWindowSmark::dropEvent(QDropEvent* event) {
    QMainWindow::dropEvent(event);
    QList<QUrl> urls = event->mimeData()->urls();
    if(! urls.empty()) {
        QString path = urls.first().toLocalFile();
        if(! _aux_cancel_current_operation())
            openMarkdownFile(path.toLocal8Bit().data());
    }
}

void GuiWindowSmark::closeEvent(QCloseEvent* event) {
    if(! _aux_cancel_current_operation())
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
    if(! _aux_cancel_current_operation()) {
        _is_modified = false;
        _aux_set_current_path(QString());
        ui->WebView->clearHtml();
        ui->EditMark->clear();
        _aux_enable_GUI();
    }
}

void GuiWindowSmark::on_ActionFileOpen_triggered(void) {
    if(! _aux_cancel_current_operation()) {
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
        _aux_set_current_path(path);
    }
    // save current markdown context
    QString error, context;
    context = ui->EditMark->toPlainText();
    if(! gSaveTextFile(_current_path, context, &error)) {
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
    if(! gSaveTextFile(_current_path, context, &error))
        QMessageBox::warning(this, tr("Error"), error);
}

void GuiWindowSmark::on_ActionFileClose_triggered(void) {
    if(! _aux_cancel_current_operation()) {
        ui->EditMark->clear();
        ui->WebView->clearHtml();
        _is_modified = false;
        _aux_set_current_path(QString());
        _aux_disable_GUI();
    }
}

void GuiWindowSmark::on_ActionFileQuite_triggered(void) {
    qApp->quit();
}

void GuiWindowSmark::on_ActionFileExport_triggered(void)
{
    // default html path
    QFileInfo file_info(_current_path);
    QString default_path = gFileDir(_current_path);
    default_path += tr("/");
    default_path += file_info.completeBaseName() + tr(".html");

    // select a path to save
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Export Current Markdown As HTML"),
                       default_path,
                       tr("html files (*.html);; all files (*.*)") );
    if(! path.isEmpty()) {
        QString context = ui->WebView->page()->mainFrame()->toHtml();
        gSaveTextFile(path, context);
    }
}

/* *****************************************************************************
 *
 *                                  Menu View
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionViewReadOnly_triggered(void) {
    _aux_switch_GUI_mode(ReadMode);
}

void GuiWindowSmark::on_ActionViewEditOnly_triggered(void) {
    _aux_switch_GUI_mode(EditMode);
}

void GuiWindowSmark::on_ActionViewPreview_triggered(void) {
    _aux_switch_GUI_mode(PreviewMode);
}

void GuiWindowSmark::on_ActionViewFullScreen_triggered(void) {
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}

void GuiWindowSmark::on_ActionViewHideMenu_triggered(void) {
    if(menuWidget()->isVisible())
        menuWidget()->hide();
    else
        menuWidget()->show();
}

void GuiWindowSmark::on_ActionViewHideToolbar_triggered(void) {
    if(ui->ToolBarNormal->isVisible()
       || ui->ToolBarEditor->isVisible()) {
        ui->ToolBarNormal->hide();
        ui->ToolBarEditor->hide();
    } else {
        ui->ToolBarNormal->show();
        if(ui->EditMark->isVisible())
            ui->ToolBarEditor->show();
    }
}

/* *****************************************************************************
 *
 *                                  Menu Edit
 *
 * ****************************************************************************/

void GuiWindowSmark::on_ActionEditFlush_triggered(void) {
    if(_CSS_is_modified) {
        gApp().CSS = ui->EditCSS->toPlainText();
        ::gSaveTextFile(gApp().CSSPath, gApp().CSS);
        _CSS_is_modified = false;
    }
    _aux_do_parse();
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
        QString selected = ui->WebView->selectedText();
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
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatItalic_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedItalic();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatUnderline_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedUnderLine();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatBigger_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedBigger();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatSmaller_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSmaller();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatSub_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSub();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatSup_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedSup();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatCode_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedCode();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatQuote_triggered(void) {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedQuote();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatAlignLeft_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedLeftAlign();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatAlignRight_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedRightAlign();
        _aux_do_parse();
    }
}

void GuiWindowSmark::on_ActionFormatAlignCenter_triggered() {
    if(focusWidget() == ui->EditMark) {
        ui->EditMark->setSelectedCenterAlign();
        _aux_do_parse();
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
                              "<h1>Smark Editor 1.0</h1>"
                              "A markdown editor bases on Qt4 & pandoc under LGPL"
                              "<br />"
                              "Copyright @ <a href=\"elerao.ao@gmail.com\">elerao.ao@gmail.com</a> 2013" );
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
    if(! gLoadTextFile(_html_cache, &context, &error)) {
        QMessageBox::warning(this, tr("Error"), error);
    } else {
        // do not use [html = gApp().Template], otherwise
        // the [reserve(...)] will be invalid...
        static QString html(16*1024, 0);
        html.reserve(16*1024);
        html.clear();
        html.prepend(gApp().Template);

        // just use MathJax if we need
        if(-1 != context.indexOf("class=\"math\"")) {
            html.replace("$$__LOCAL_MATH_JAX_PATH__$$", gApp().MathJaxPath);
        }
        html.replace("$$__CSS_TEXT__$$", gApp().CSS);
        html.replace("$$__HTML_TEXT__$$", context);

        // update webview, do not use update()
        int position = ui->WebView->scrollValue();
        ui->WebView->setHtml(html, QUrl::fromLocalFile(_current_path));
        ui->WebView->repaint(ui->WebView->page()->mainFrame()->geometry());
        ui->WebView->setScrollValue(position);

        // remove cache file
        gRemoveFile(_html_cache);
        gRemoveFile(_mark_cache);
    }
}

void GuiWindowSmark::when_mark_is_edit(void) {
    _is_modified = true;
}
void GuiWindowSmark::when_css_is_edit(void) {
    _CSS_is_modified = true;
}
