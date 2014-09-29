#ifndef SMARK_SMK_OPTION_DIALOG_H
#define SMARK_SMK_OPTION_DIALOG_H

#include "Smark/SmkConfig.h"
#include "Smark/SmkHtmlHighlighter.h"

namespace Ui {
    class SmkOptionDialog;
}

class SmkOptionDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SmkOptionDialog(QWidget *parent = 0);
    ~SmkOptionDialog();

public:

    void initGui();

private slots:

    void on_comboBoxGuiStyle_currentIndexChanged(const QString&);
    void on_comboBoxEnglishFont_currentFontChanged(const QFont&);
    void on_spinBoxFontSize_valueChanged(int);
    void on_listColor_itemDoubleClicked(QListWidgetItem*);
    void on_buttonCSSURL_clicked();
    void on_buttonMathJaxURL_clicked();
    void on_buttonBox_accepted();
    void on_lineEditCSSURL_textEdited(const QString& arg);
    void on_lineEditMathJaxURL_textEdited(const QString& arg);
    void on_buttonDefault_clicked();
private:

    Ui::SmkOptionDialog* ui;
    SmkHtmlHighlighter*  pHighlighter_[4];
};

#endif // SMARK_SMK_OPTION_DIALOG_H
