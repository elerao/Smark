#ifndef SMARK_SMK_MARK_WIDGET_H
#define SMARK_SMK_MARK_WIDGET_H

#include "Smark/SmkBasicMarkWidget.h"

class SmkMarkWidget
        : public SmkBasicMarkWidget
{
    Q_OBJECT

public:

    explicit SmkMarkWidget(QWidget* parent = 0);

/* *****************************************************************************
 *
 *                          synchronized scrolling
 *
 * ****************************************************************************/

public:

    float scrollRatio(void);

private slots:

    void when_VerticalScrollBar_valueChanged(int val);

signals:

    void verticalScroll(float ratio);

/* *****************************************************************************
 *
 *                              find and replace
 *
 * ****************************************************************************/

public:

    void findPrevious(const QString& text, bool cased, bool whole);
    void findNext(const QString& text, bool cased, bool whole);
    void replaceCurrent(const QString& new_text);
    void replaceAll(const QString& old_text,
                    const QString& new_text,
                    bool cased, bool whole);

/* *****************************************************************************
 *
 *                              set selected format
 *
 * ****************************************************************************/

public: // set selected format

    void setSelectedFormat(const QString& before, const QString& after);
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

/* *****************************************************************************
 *
 *                        insert context template
 *
 * ****************************************************************************/

public: // insert object

    void insertImage(void);
    void insertLink(void);
    void insertMathJax(void);

/* *****************************************************************************
 *
 *                              event porcess
 *
 * ****************************************************************************/

protected: // event porcess

    void keyPressEvent(QKeyEvent*);
};

#endif // SMARK_SMK_MARK_WIDGET_H
