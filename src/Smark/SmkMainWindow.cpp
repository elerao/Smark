#include <QtGui>
#include <QtWebKit>
#include "Smark/SmkIO.h"
#include "Smark/SmkMainWindow.h"
#include "Smark/SmkOptionDialog.h"
#include "ui_SmkMainWindow.h"

SmkMainWindow::SmkMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::SmkMainWindow)
{
    ui->setupUi(this);
    pHtmlEditor_ = new QPlainTextEdit(NULL);
    pHtmlHighlighter_ = new SmkHtmlHighlighter(pHtmlEditor_->document());

    _aux_initGUI();
    _aux_disableGUI();
    _aux_connectSignalAndSlot();

    markCachePath_ = qMarkCachePath();
    htmlCachePath_ = qHtmlCachePath();
    isModified_     = false;
}

SmkMainWindow::~SmkMainWindow() {
    pHtmlEditor_->hide();
    if(ui->toolBar->isHidden())
        qSmkApp()->setOption("gui.toolbar", "FALSE");
    else
        qSmkApp()->setOption("gui.toolbar", "TRUE");
    delete ui;
    delete pHtmlEditor_;
}

/* *****************************************************************************
 *
 *                             open a markdown file
 *
 * ****************************************************************************/

void SmkMainWindow::openMarkdownFile(const QString& path) {
    QString context;
    QString errorInfo = qSmkLoadText(path, &context);
    if(errorInfo != SMK_EMPTY_STR) {
        QMessageBox::warning(this, tr("Error"), errorInfo);
        _aux_disableGUI();
    } else {
        _aux_enableGUI();
        ui->markView->setPlainText(context);
        isModified_ = false;
        _aux_setCurrentPath(path);
        on_actionFlush_triggered();
        pHtmlEditor_->hide();
    }
}

/* *****************************************************************************
 *
 *                             aux function
 *
 * ****************************************************************************/

void SmkMainWindow::_aux_connectSignalAndSlot(void) {
    connect(ui->markView, SIGNAL(textChanged()),
            this,         SLOT(when_mark_is_edit()) );
    connect(&parser_,     SIGNAL(completed()),
            this,         SLOT(when_parser_process_finish()) );
    connect(ui->markView, SIGNAL(verticalScroll(float)),
            ui->htmlView,  SLOT(setScrollRatio(float)) );
}

void SmkMainWindow::_aux_switchDisplayMode(SmkGuiMode mode) {
    if(height() > 0.73*width()) {
        // 竖排模式
        ui->splitter->setOrientation(Qt::Vertical);
        if(mode == eSmkEditMode) {
            QList<int> list;
            list.append(0);
            list.append(ui->widget->height());
            ui->splitter->setSizes(list);
        } else if(mode == eSmkReadMode) {
            QList<int> list;
            list.append(ui->widget->height());
            list.append(0);
            ui->splitter->setSizes(list);
        } else {
            QList<int> list;
            list.append(ui->widget->height()/2);
            list.append(ui->widget->height()/2);
            ui->splitter->setSizes(list);
        }//if..else..
    } else {
        // 横排模式
        ui->splitter->setOrientation(Qt::Horizontal);
        if(mode == eSmkEditMode) {
            QList<int> list;
            list.append(0);
            list.append(ui->widget->width());
            ui->splitter->setSizes(list);
        } else if(mode == eSmkReadMode) {
            QList<int> list;
            list.append(ui->widget->width());
            list.append(0);
            ui->splitter->setSizes(list);
        } else {
            QList<int> list;
            list.append(ui->widget->width()/2);
            list.append(ui->widget->width()/2);
            ui->splitter->setSizes(list);
        }//if..else..
    }//f(height() > 0.73*width()...
    currentGuiMode_ = mode;
}

void SmkMainWindow::_aux_initGUI(void) {
    ui->toolBar->setIconSize(QSize(16, 16));
    ui->toolBar->setVisible("TRUE" == qSmkApp()->option("gui.toolbar"));
    _aux_switchDisplayMode(eSmkReadMode);
    ui->widgetFind->hide();

    QFont font;
    font.setFamily(qSmkApp()->option("font.family"));
    font.setStyleHint(QFont::Monospace, QFont::PreferAntialias);
    pHtmlEditor_->setFont(font);
    pHtmlEditor_->setWindowIcon(QIcon(":/LOGO.png"));
    pHtmlEditor_->setWindowTitle("HTML Source");
    pHtmlEditor_->setReadOnly(true);
    pHtmlEditor_->hide();
}

void SmkMainWindow::_aux_enableGUI(void) {
    // center widget
    ui->widget->setEnabled(true);

    // file menu
    ui->actionSave->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionPrint->setEnabled(true);

    // edit menu
    ui->actionFlush->setEnabled(true);
    ui->actionViewHTML->setEnabled(true);
    ui->actionCut->setEnabled(true);
    ui->actionCopy->setEnabled(true);
    ui->actionPaste->setEnabled(true);
    ui->actionRedo->setEnabled(true);
    ui->actionUndo->setEnabled(true);
    ui->actionFind->setEnabled(true);

    // insert menu
    ui->actionInsertImage->setEnabled(true);
    ui->actionInsertLink->setEnabled(true);
    ui->actionInsertMathJax->setEnabled(true);

    // format menu
    ui->actionFormatBlod->setEnabled(true);
    ui->actionFormatItalic->setEnabled(true);
    ui->actionFormatUnderline->setEnabled(true);
    ui->actionFormatBigger->setEnabled(true);
    ui->actionFormatSmaller->setEnabled(true);
    ui->actionFormatSub->setEnabled(true);
    ui->actionFormatSup->setEnabled(true);
    ui->actionFormatCode->setEnabled(true);
    ui->actionFormatQuote->setEnabled(true);
    ui->actionAlignCenter->setEnabled(true);
    ui->actionAlignLeft->setEnabled(true);
    ui->actionAlignRight->setEnabled(true);
}

void SmkMainWindow::_aux_disableGUI(void) {
    // center widget
    ui->widget->setEnabled(false);

    // file menu
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionPrint->setEnabled(false);

    // edit menu
    ui->actionFlush->setEnabled(false);
    ui->actionViewHTML->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionFind->setEnabled(false);

    // insert menu
    ui->actionInsertImage->setEnabled(false);
    ui->actionInsertLink->setEnabled(false);
    ui->actionInsertMathJax->setEnabled(false);

    // format menu
    ui->actionFormatBlod->setEnabled(false);
    ui->actionFormatItalic->setEnabled(false);
    ui->actionFormatUnderline->setEnabled(false);
    ui->actionFormatBigger->setEnabled(false);
    ui->actionFormatSmaller->setEnabled(false);
    ui->actionFormatSub->setEnabled(false);
    ui->actionFormatSup->setEnabled(false);
    ui->actionFormatCode->setEnabled(false);
    ui->actionFormatQuote->setEnabled(false);
    ui->actionAlignCenter->setEnabled(false);
    ui->actionAlignLeft->setEnabled(false);
    ui->actionAlignRight->setEnabled(false);

    // hide find widget
    ui->widgetFind->hide();

    // hide HTML View
    pHtmlEditor_->hide();
}

void SmkMainWindow::_aux_setCurrentPath(const QString& path) {
    currentPath_ = path;
    markCachePath_   = qMarkCachePath(path);
    htmlCachePath_   = qHtmlCachePath(path);
    if(path.isEmpty()) {
        setWindowTitle("Smark");
    } else {
        QString title =  "Smark - " + QFileInfo(path).fileName();
        setWindowTitle(title);
    }
}

bool SmkMainWindow::_aux_cancelCurrentOperation(void) {
    const bool Cancel = true;
    const bool   GoOn = false;
    if(isModified_ == false) return GoOn;

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
        if(currentPath_.isEmpty()) {
            QString path = QFileDialog::getSaveFileName(
                               this,
                               tr("Save Current Markdown File"),
                               tr("~/") );
            if(path.isEmpty()) return Cancel;
            _aux_setCurrentPath(path);
        }
        // save current markdown context
        QString   context = ui->markView->toPlainText();
        QString errorInfo = qSmkSaveText(currentPath_, context);
        if(errorInfo != SMK_EMPTY_STR) {
            QMessageBox::warning(this, tr("Error"), errorInfo);
            return Cancel;
        } else {
            isModified_ = false;
            return GoOn;
        }
    }// if...else if...
    return GoOn;
}

void SmkMainWindow::_aux_parseMarkdownToHTML(void) {
    QString   context = ui->markView->toPlainText();
    QString errorInfo = qSmkSaveText(markCachePath_, context);
    if(errorInfo != SMK_EMPTY_STR) {
        QMessageBox::warning(this, tr("Write Cache File Error"), errorInfo);
    } else {
        QString cssPath = qSmkApp()->option("url.css");
        parser_.parseMarkToHtml(markCachePath_, htmlCachePath_, cssPath);
    }
}

/* *****************************************************************************
 *
 *                              event process
 *
 * ****************************************************************************/

void SmkMainWindow::resizeEvent(QResizeEvent* event) {
    // do not call QMainWindow::resizeEvent(event);
    QMainWindow::resizeEvent(event);
    _aux_switchDisplayMode(currentGuiMode_);
}

void SmkMainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape) {
        // Omnipotent Esc Key
        if(ui->widgetFind->isVisible()) {
            // 如果查找部件正在显示则隐藏查找部件
            ui->widgetFind->hide();
            ui->markView->setFocus();
        } else if(ui->markView->isVisible()) {
            // 如果 Markdown 编辑器正在显示则隐藏 Markdown 编辑器
            _aux_switchDisplayMode(eSmkReadMode);
        }
    }
    QMainWindow::keyPressEvent(event);
}

void SmkMainWindow::dragEnterEvent(QDragEnterEvent* event) {
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

void SmkMainWindow::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if(false == urls.empty()) {
        QString path = urls.first().toLocalFile();
        if(false == _aux_cancelCurrentOperation()) {
            openMarkdownFile(path);
        }
    }
    QMainWindow::dropEvent(event);
}

void SmkMainWindow::closeEvent(QCloseEvent* event) {
    if(false == _aux_cancelCurrentOperation())
        event->accept();
    else
        event->ignore();
}

/* *****************************************************************************
 *
 *                             文件菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionNew_triggered() {
    if(false == _aux_cancelCurrentOperation()) {
        isModified_ = false;
        _aux_setCurrentPath(QString());
        ui->htmlView->clearHtml();
        ui->markView->clear();
        _aux_enableGUI();
    }
}

void SmkMainWindow::on_actionOpen_triggered() {
    if(false == _aux_cancelCurrentOperation()) {
        QString path = QFileDialog::getOpenFileName(
                           this,
                           tr("Open File"),
                           QFileInfo(currentPath_).path(),
                           tr("markdown files (*.markdown *.md *mkd);; all files (*.*)"));
        if(path.isEmpty()) return;
        openMarkdownFile(path);
    }
}

void SmkMainWindow::on_actionSave_triggered() {
    // if current path is empty, select a path to save
    if(currentPath_.isEmpty()) {
        QString path = QFileDialog::getSaveFileName(
                           this,
                           tr("Save Current Markdown File"),
                           tr("~/"),
                           tr("markdown files (*.markdown *.md *mkd);; all files (*.*)"));
        if(path.isEmpty()) return;
        _aux_setCurrentPath(path);
    }

    // save current markdown context
    QString   context = ui->markView->toPlainText();
    QString errorInfo = qSmkSaveText(currentPath_, context);
    if(errorInfo != SMK_EMPTY_STR) {
        QMessageBox::warning(this, tr("Error"), errorInfo);
    } else if(isModified_ == true) {
        isModified_ = false;
        if(windowTitle().startsWith("* ")) // 去除窗口标题的 dirty 标记
            setWindowTitle(windowTitle().remove(0, 2));
        on_actionFlush_triggered();
    }
}

void SmkMainWindow::on_actionSaveAs_triggered()
{
    // get export file path
    QString path = QFileDialog::getSaveFileName(
                       this,
                       tr("Save Current Markdown As Given Format"),
                       currentPath_,
                       tr("markdown files (*.markdown *.md);;"
                          "HTML files (*.html);;"
                          "Microsoft Word (*.docx);;"
                          "Portable Document Format (*.pdf);;"
                          "Tex Text (*.tex);;"
                          "OpenOffice ODT (*.odt);;"
                          "reStructured Text (*.rst);;"
                          "Wiki Markup (*.wiki);;"
                          "EPUB ebook (*.epub);;"
                          "Plain Text (*.txt);;"
                          "all files (*.*)") );
    if(path.isEmpty()) return;

    QString suffix = QFileInfo(path).suffix().toLower();
    if(suffix == "md" || suffix == "markdown") {
        qSmkSaveText(path, ui->markView->toPlainText());
        return;
    } else if(suffix == "html") {
        qSmkSaveText(path, htmlContext_);
        return;
    } else if(suffix == "pdf") {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(path);
        ui->htmlView->print(&printer);
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
        parser_.parse(currentPath_, "markdown",
                      path,          format,
                      qSmkApp()->option("url.css"));
    }
}

void SmkMainWindow::on_actionClose_triggered() {
    if(false == _aux_cancelCurrentOperation()) {
        ui->markView->clear();
        ui->htmlView->clearHtml();
        isModified_ = false;
        _aux_setCurrentPath(QString());
        _aux_disableGUI();
    }
}

void SmkMainWindow::on_actionOption_triggered() {
    SmkOptionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        ui->markView->updateOption();
        // 不更新 HTML 视图，由用户自己刷新
        // ui->htmlView->settings()->clearMemoryCaches();
        // _aux_parseMarkdownToHTML();

        // 确定之后再保存文件设置
        qSmkApp()->saveOption();
    }
}

void SmkMainWindow::on_actionPrint_triggered() {
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Accepted) {
        printer.setOutputFormat(QPrinter::NativeFormat);
        ui->htmlView->print(&printer);
    }
}

void SmkMainWindow::on_actionQuite_triggered() {
    if(false == _aux_cancelCurrentOperation()) {
        ui->markView->clear();
        ui->htmlView->clearHtml();
        isModified_ = false;
        _aux_setCurrentPath(QString());
        _aux_disableGUI();
        qSmkApp()->quit();
    }
}

/* *****************************************************************************
 *
 *                             视图菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionPreview_triggered() {
    _aux_switchDisplayMode(eSmkPreviewMode);
}

void SmkMainWindow::on_actionReadOnly_triggered() {
    _aux_switchDisplayMode(eSmkReadMode);
}

void SmkMainWindow::on_actionEditOnly_triggered() {
    _aux_switchDisplayMode(eSmkEditMode);
}

void SmkMainWindow::on_actionFullScreen_triggered(bool checked) {
    if(checked)
        showFullScreen();
    else
        showNormal();
}

/* *****************************************************************************
 *
 *                             编辑菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionFlush_triggered() {
    ui->htmlView->settings()->clearMemoryCaches();
    _aux_parseMarkdownToHTML();
}

void SmkMainWindow::on_actionCut_triggered() {
    QWidget* focus = focusWidget();
    if(focus == ui->markView)
        ui->markView->cut();
}

void SmkMainWindow::on_actionCopy_triggered() {
    QWidget* focus = focusWidget();
    if(focus == ui->markView) {
        ui->markView->copy();
    } else {
        QString      selected = ui->htmlView->selectedText();
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(selected);
    }
}

void SmkMainWindow::on_actionPaste_triggered() {
    QWidget* focus = focusWidget();
    if(focus == ui->markView)
        ui->markView->paste();
}

void SmkMainWindow::on_actionRedo_triggered() {
    QWidget* focus = focusWidget();
    if(focus == ui->markView)
        ui->markView->redo();
}

void SmkMainWindow::on_actionUndo_triggered() {
    QWidget* focus = focusWidget();
    if(focus == ui->markView)
        ui->markView->undo();
}

void SmkMainWindow::on_actionFind_triggered() {
    if(ui->markView->isVisible()) {
        QString selected = ui->markView->textCursor().selectedText();
        ui->lineEditFind->setText(selected);
        ui->buttonFindNext->setFocus();
        ui->widgetFind->show();
    }
}

void SmkMainWindow::on_actionViewHTML_triggered() {
    // copy to clipboard
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(htmlContext_);

    // 显示一个 HTML 代码预览窗口
    pHtmlEditor_->setPlainText(htmlContext_);
    pHtmlEditor_->resize(ui->htmlView->size());
    pHtmlEditor_->show();
}

/* *****************************************************************************
 *
 *                             插入菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionInsertImage_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->insertImage();
}

void SmkMainWindow::on_actionInsertLink_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->insertLink();
}

void SmkMainWindow::on_actionInsertMathJax_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->insertMathJax();
}

/* *****************************************************************************
 *
 *                             格式菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionFormatBlod_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedBlod();
}

void SmkMainWindow::on_actionFormatItalic_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedItalic();
}

void SmkMainWindow::on_actionFormatUnderline_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedUnderLine();
}

void SmkMainWindow::on_actionFormatBigger_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedBigger();
}

void SmkMainWindow::on_actionFormatSmaller_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedSmaller();
}

void SmkMainWindow::on_actionFormatSub_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedSub();
}

void SmkMainWindow::on_actionFormatSup_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedSup();
}

void SmkMainWindow::on_actionFormatCode_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedCode();
}

void SmkMainWindow::on_actionFormatQuote_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedQuote();
}

void SmkMainWindow::on_actionAlignLeft_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedLeftAlign();
}

void SmkMainWindow::on_actionAlignRight_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedRightAlign();
}

void SmkMainWindow::on_actionAlignCenter_triggered() {
    if(focusWidget() == ui->markView)
        ui->markView->setSelectedCenterAlign();
}

/* *****************************************************************************
 *
 *                             帮助菜单事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_actionAboutQt_triggered() {
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void SmkMainWindow::on_actionAboutSmark_triggered() {
    QString aboutSmark = tr("<h1>Smark Editor 2.0</h1>"
                            "A sample markdown editor under <b>GPL</b>(the GNU General Public License) "
                            "based on Qt 4.8, pandoc & MathJax"
                            "<p align=\"center\"><b>Copyright (C) 2013~2014 Elerao Ao</b></p>"
                            "This program is free software: you can redistribute it and/or modify"
                            "it under the terms of the GNU General Public License as published by"
                            "the Free Software Foundation, either version 3 of the License, or"
                            "(at your option) any later version. This program is distributed in"
                            "the hope that it will be useful, but WITHOUT ANY WARRANTY; without"
                            "even the implied warranty of MERCHANTABILITY or FITNESS FOR A "
                            "PARTICULAR PURPOSE.  See the GNU General Public License for more "
                            "details. You should have received a copy of the GNU General Public"
                            "License along with this program. If not, see "
                            "<a href=\"http://www.gnu.org/licenses\">http://www.gnu.org/licenses/</a><br/>."
                            "<p align=\"right\">Contact : <a href=\"elerao.ao@gmail.com\">elerao.ao@gmail.com</a></p>" );
    QMessageBox::about(this, tr("About Smark"), aboutSmark);
}

/* *****************************************************************************
 *
 *                             查找部件事件响应
 *
 * ****************************************************************************/

void SmkMainWindow::on_buttonFindPrev_clicked() {
    ui->markView->findPrevious(ui->lineEditFind->text(),
                               ui->checkBoxCase->isChecked(),
                               ui->checkBoxWholeWord->isChecked());
}

void SmkMainWindow::on_buttonFindNext_clicked() {
    ui->markView->findNext(ui->lineEditFind->text(),
                           ui->checkBoxCase->isChecked(),
                           ui->checkBoxWholeWord->isChecked());
}

void SmkMainWindow::on_buttonReplace_clicked() {
    ui->markView->replaceCurrent(ui->lineEditReplace->text());
}

void SmkMainWindow::on_buttonReplaceAll_clicked() {
    ui->markView->replaceAll(ui->lineEditFind->text(),
                             ui->lineEditReplace->text(),
                             ui->checkBoxCase->isChecked(),
                             ui->checkBoxWholeWord->isChecked() );
}

/* *****************************************************************************
 *
 *                             后台调用 Pandoc 相关
 *
 * ****************************************************************************/

void SmkMainWindow::when_parser_process_finish(void) {
    QString context;
    QString errorInfo = qSmkLoadText(htmlCachePath_, &context);
    if(errorInfo != SMK_EMPTY_STR) {
        QMessageBox::warning(this, tr("Error"), errorInfo);
    } else {
        // update webview, do not use update()
        htmlContext_.swap(context);

        // replace IPV6 CDN or IPV4 CND as user defined mathjax url
        htmlContext_.replace("https://d3eoax9i5htok0.cloudfront.net/mathjax/latest/MathJax.js",
                      qSmkApp()->option("url.mathjax"));
        htmlContext_.replace("http://cdn.mathjax.org/mathjax/latest/MathJax.js",
                      qSmkApp()->option("url.mathjax"));

        // use SVG render MathJax is much faster
        htmlContext_.replace("TeX-AMS-MML_HTMLorMML", "TeX-AMS-MML_SVG");
        htmlContext_.insert(htmlContext_.indexOf("</head>"), qSmkApp()->option("text.head"));
        htmlContext_.insert(htmlContext_.indexOf("<body>")+6, qSmkApp()->option("text.hat"));
        htmlContext_.insert(htmlContext_.indexOf("</body>"), qSmkApp()->option("text.tail"));
        htmlContext_.insert(htmlContext_.indexOf("</body>")+7, qSmkApp()->option("text.foot"));

        int position = ui->htmlView->scrollValue();
        ui->htmlView->setHtml(htmlContext_, QUrl::fromLocalFile(currentPath_));
        ui->htmlView->repaint(ui->htmlView->page()->mainFrame()->geometry());
        ui->htmlView->setScrollValue(position);

        // remove cache file
        qSmkRemoveFile(htmlCachePath_);
        qSmkRemoveFile(markCachePath_);
    }
}

void SmkMainWindow::when_mark_is_edit(void) {
    if(ui->markView->isEnabled()) {
        isModified_ = true;
        if(! windowTitle().startsWith("* ")) // 向窗口标题添加 dirty 标记
            setWindowTitle("* " + windowTitle());
    }
}
