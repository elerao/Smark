#ifndef GUI_MARK_EDIT_H
#define GUI_MARK_EDIT_H

#include <QPlainTextEdit>

class GuiMarkEdit
        : public QPlainTextEdit
{
    Q_OBJECT

public:

    explicit GuiMarkEdit(QWidget *parent = 0);

public: // synchronized scrolling

    float scrollRatio(void);

private slots:

    void when_VerticalScrollBar_valueChanged(int val);

signals:

    void verticalScroll(float ratio);

public: // find and replace

    void findPrevious(const QString& text, bool cased, bool whole);
    void findNext(const QString& text, bool cased, bool whole);
    void replaceCurrent(const QString& new_text);
    void replaceAll(const QString& old_text,
                    const QString& new_text,
                    bool cased, bool whole);

public: // set selected format

    void setSelectedBlod(void);
    void setSelectedItalic(void);
    void setSelectedUnderLine(void);
    void setSelectedDeleted(void);
    void setSelectedQuote(void);
    void setSelectedCode(void);
    void setSelectedSub(void);
    void setSelectedSup(void);
    void setSelectedBigger(void);
    void setSelectedSmaller(void);
    void setSelectedOrderedList(void);
    void setSelectedUnorderedList(void);
    void setSelectedLeftAlign(void);
    void setSelectedRightAlign(void);
    void setSelectedCenterAlign(void);

public: // insert object

    void insertImage(void);
    void insertLink(void);
    void insertFormation(void);

private:

    void _aux_set_words_format(const QString& before, const QString& after);

protected: // event porcess

    void keyPressEvent(QKeyEvent*);

};

#endif // GUI_MARK_EDIT_H
