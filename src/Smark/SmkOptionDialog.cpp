#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"
#include "Smark/SmkHtmlHighlighter.h"
#include "Smark/SmkOptionDialog.h"
#include "ui_SmkOptionDialog.h"

SmkOptionDialog::SmkOptionDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SmkOptionDialog)
{
    ui->setupUi(this);
    initGui();
}

SmkOptionDialog::~SmkOptionDialog() {
    delete ui;
    delete pHighlighter_[0];
    delete pHighlighter_[1];
    delete pHighlighter_[2];
    delete pHighlighter_[3];
}

void SmkOptionDialog::initGui()
{
    // Gui Style
    QString         style = qSmkApp()->option("gui.style");
    QStringList styleList = QStyleFactory::keys();
    ui->comboBoxGuiStyle->clear();
    ui->comboBoxGuiStyle->addItems(styleList);
    ui->comboBoxGuiStyle->setCurrentIndex(styleList.indexOf(style));

    // Font Options
    QStringList fontFamily = qSmkApp()->option("font.family").split(",");
    if(fontFamily.size() == 2) {
        ui->comboBoxEnglishFont->setCurrentFont(QFont(fontFamily[0]));
        ui->comboBoxChineseFont->setCurrentFont(QFont(fontFamily[1]));
    } else if(fontFamily.size() == 1) {
        ui->comboBoxEnglishFont->setCurrentFont(QFont(fontFamily[0]));
        ui->comboBoxChineseFont->setCurrentFont(QFont(fontFamily[0]));
    }
    ui->spinBoxFontSize->setValue(qSmkApp()->option("font.size").toInt());

    // Syntax Color
    ui->listColor->clear();
    ui->listColor->setFont(QFont(qSmkApp()->option("font.family"),
                                 qSmkApp()->option("font.size").toInt()));
    QListWidgetItem* pItem = NULL;

    pItem = new QListWidgetItem("Background");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.foreground")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Foreground");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.foreground")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Focusline");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.focusline")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.foreground")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Focusword");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.focusword")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.foreground")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Header");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.header")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Code");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.code")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Latex");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.latex")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("HTML");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.html")));
    ui->listColor->addItem(pItem);

    pItem = new QListWidgetItem("Link");
    pItem->setBackgroundColor(QColor(qSmkApp()->option("color.background")));
    pItem->setTextColor(QColor(qSmkApp()->option("color.link")));
    ui->listColor->addItem(pItem);

    // URL setting
    QString cssURL = qSmkApp()->option("url.css");
    if(QFileInfo(cssURL).exists()==false && cssURL.startsWith("http")==false) {
        // 如果 cssURL 不存在则将其标记为红色
        QPalette palette = ui->lineEditCSSURL->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEditCSSURL->setPalette(palette);
    }
    if(cssURL.startsWith(qSmkApp()->applicationDirPath()))
        cssURL.replace(qSmkApp()->applicationDirPath(), ".");
    ui->lineEditCSSURL->setText(cssURL);

    QString mathJaxURL = qSmkApp()->option("url.mathjax");
    if(QFileInfo(mathJaxURL).exists()==false && mathJaxURL.startsWith("http")==false) {
        // 如果 MathJaxURL 不存在则将其标记为红色
        QPalette palette = ui->lineEditMathJaxURL->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEditMathJaxURL->setPalette(palette);
    }
    if(mathJaxURL.startsWith(qSmkApp()->applicationDirPath()))
        mathJaxURL.replace(qSmkApp()->applicationDirPath(), ".");
    ui->lineEditMathJaxURL->setText(mathJaxURL);

    // context setting
    ui->textEditHeader->setPlainText(qSmkApp()->option("text.head"));
    ui->textEditHat->setPlainText(qSmkApp()->option("text.hat"));
    ui->textEditTail->setPlainText(qSmkApp()->option("text.tail"));
    ui->textEditFoot->setPlainText(qSmkApp()->option("text.foot"));

    pHighlighter_[0] = new SmkHtmlHighlighter(ui->textEditHeader->document());
    pHighlighter_[1] = new SmkHtmlHighlighter(ui->textEditHat->document());
    pHighlighter_[2] = new SmkHtmlHighlighter(ui->textEditTail->document());
    pHighlighter_[3] = new SmkHtmlHighlighter(ui->textEditFoot->document());
}

void SmkOptionDialog::on_comboBoxGuiStyle_currentIndexChanged(const QString& style) {
    qSmkApp()->setStyle(style);
}

void SmkOptionDialog::on_comboBoxEnglishFont_currentFontChanged(const QFont& font) {
    QFont currentFont = font;
    currentFont.setPointSize(ui->spinBoxFontSize->value());
    ui->listColor->setFont(currentFont);
}

void SmkOptionDialog::on_spinBoxFontSize_valueChanged(int fontSize) {
    QFont currentFont = ui->comboBoxEnglishFont->font();
    currentFont.setPointSize(fontSize);
    ui->listColor->setFont(currentFont);
}

void SmkOptionDialog::on_listColor_itemDoubleClicked(QListWidgetItem* pItem)
{
    QString text = pItem->text();
    if(text == "Background" || text == "Focusline" || text == "Focusword") {
        // 选择背景色
        QColor color = pItem->backgroundColor();
        color = QColorDialog::getColor(color,
                                       this,
                                       tr("Choose Color for ") + text);
        if(! color.isValid()) return;
        pItem->setBackgroundColor(color);
        if(text == "Background") {
            // 需要更新整体的背景色
            for(int i=0; i<ui->listColor->count(); i++) {
                if(ui->listColor->item(i)->text() != "Focusword" &&
                   ui->listColor->item(i)->text() != "Focusline" ) {
                    ui->listColor->item(i)->setBackgroundColor(color);
                }
            }//for..
        }//if(..
    } else {
        // 选择前景色
        QColor color = pItem->textColor();
        color = QColorDialog::getColor(color,
                                       this,
                                       tr("Choose Color for ") + text);
        if(! color.isValid()) return;
        pItem->setTextColor(color);
        if(text == "Foreground") {
            // 需要更新整体的前景色
            for(int i=0; i<ui->listColor->count(); i++) {
                if(ui->listColor->item(i)->text() == "Background" ||
                   ui->listColor->item(i)->text() == "Focusword" ||
                   ui->listColor->item(i)->text() == "Focusline" ) {
                    ui->listColor->item(i)->setTextColor(color);
                }
            }//for..
        }//if(..
    }
    update();
}

void SmkOptionDialog::on_buttonCSSURL_clicked() {
    QString path = QFileDialog::getOpenFileName(
                       this,
                       trUtf8("Choose Style Sheet File URL"),
                       qSmkApp()->applicationDirPath(),
                       trUtf8("Style Sheeet (*.css)") );
    if(path.isEmpty()) return;
    if(path.startsWith(qSmkApp()->applicationDirPath()))
        path.replace(qSmkApp()->applicationDirPath(), ".");
    ui->lineEditCSSURL->setText(path);
}

void SmkOptionDialog::on_buttonMathJaxURL_clicked() {
    QString path = QFileDialog::getOpenFileName(
                       this,
                       trUtf8("Choose MathJax File URL"),
                       qSmkApp()->applicationDirPath(),
                       trUtf8("JavaScript (*.js)") );
    if(path.isEmpty()) return;
    if(path.startsWith(qSmkApp()->applicationDirPath()))
        path.replace(qSmkApp()->applicationDirPath(), ".");
    ui->lineEditMathJaxURL->setText(path);
}

void SmkOptionDialog::on_buttonBox_accepted()
{
    // gui color options
    qSmkApp()->setOption("gui.style", ui->comboBoxGuiStyle->currentText());
    qSmkApp()->setOption("font.size", QString::number(ui->spinBoxFontSize->value()));
    QString fontFamily = ui->comboBoxEnglishFont->currentFont().family() + "," +
                         ui->comboBoxChineseFont->currentFont().family();
    qSmkApp()->setOption("font.family",  fontFamily);

    // syntax color options
    for(int i=0; i<ui->listColor->count(); i++) {
        QListWidgetItem* pItem = ui->listColor->item(i);
        if(pItem->text() == "Background")
            qSmkApp()->setOption("color.background", pItem->backgroundColor().name());
        else if(pItem->text() == "Foreground")
            qSmkApp()->setOption("color.foreground", pItem->textColor().name());
        else if(pItem->text() == "Focusline")
            qSmkApp()->setOption("color.focusline", pItem->backgroundColor().name());
        else if(pItem->text() == "Focusword")
            qSmkApp()->setOption("color.focusword", pItem->backgroundColor().name());
        else if(pItem->text() == "Header")
            qSmkApp()->setOption("color.header", pItem->textColor().name());
        else if(pItem->text() == "Code")
            qSmkApp()->setOption("color.code", pItem->textColor().name());
        else if(pItem->text() == "Latex")
            qSmkApp()->setOption("color.latex", pItem->textColor().name());
        else if(pItem->text() == "HTML")
            qSmkApp()->setOption("color.html", pItem->textColor().name());
        else if(pItem->text() == "Link")
            qSmkApp()->setOption("color.link", pItem->textColor().name());
    }//for..

    // mathjax url options
    QString mathJaxURL = ui->lineEditMathJaxURL->text();
    if(mathJaxURL.startsWith("./"))
        mathJaxURL.replace(0, 1, qSmkApp()->applicationDirPath());
    qSmkApp()->setOption("url.mathjax", mathJaxURL);

    // CSS url options
    QString cssURL = ui->lineEditCSSURL->text();
    if(cssURL.startsWith("./"))
        cssURL.replace(0, 1, qSmkApp()->applicationDirPath());
    qSmkApp()->setOption("url.css", cssURL);

    // context options
    qSmkApp()->setOption("text.head",   ui->textEditHeader->toPlainText());
    qSmkApp()->setOption("text.hat",    ui->textEditHat->toPlainText());
    qSmkApp()->setOption("text.tail",   ui->textEditTail->toPlainText());
    qSmkApp()->setOption("text.foot",   ui->textEditFoot->toPlainText());
}

void SmkOptionDialog::on_lineEditCSSURL_textEdited(const QString& arg) {
    QString url = arg;
    if(url.startsWith("./"))
        url.replace(0, 1, qSmkApp()->applicationDirPath());
    if(QFileInfo(url).exists() || url.startsWith("http")) {
        // 如果 url 对应的文件存在或为在线文件则恢复其为默认颜色
        QPalette palette = this->palette();
        ui->lineEditCSSURL->setPalette(palette);
    } else {
        // 如果 url 对应的文件不存在则将其标记为红色
        QPalette palette = ui->lineEditCSSURL->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEditCSSURL->setPalette(palette);
    }
}

void SmkOptionDialog::on_lineEditMathJaxURL_textEdited(const QString& arg) {
    QString url = arg;
    if(url.startsWith("./"))
        url.replace(0, 1, qSmkApp()->applicationDirPath());
    if(QFileInfo(url).exists() || url.startsWith("http")) {
        // 如果 url 对应的文件存在或为在线文件则恢复其为默认颜色
        QPalette palette = this->palette();
        ui->lineEditCSSURL->setPalette(palette);
    } else {
        // 如果 url 对应的文件不存在则将其标记为红色
        QPalette palette = ui->lineEditCSSURL->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEditCSSURL->setPalette(palette);
    }
}
