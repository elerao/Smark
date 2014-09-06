#include "Smark/SmkConfig.h"
#include "Smark/SmkApp.h"
#include "Smark/SmkMarkHighlighter.h"

SmkMarkHighlighter::SmkMarkHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent) {
    updateOption();
}

void SmkMarkHighlighter::updateOption(void)
{
    format_.clear();
    regexp_.clear();

    QFont normalFont(qSmkApp()->option("font.family"),
                     qSmkApp()->option("font.size").toInt());
    QFont boldFont   = normalFont; boldFont.setBold(true);
    QFont italicFont = normalFont; italicFont.setItalic(true);

    QTextCharFormat boldFormat;
    boldFormat.setFont(boldFont);
    format_.append(boldFormat);
    regexp_.append(QRegExp("<b>.*</b>"));
    format_.append(boldFormat);
    regexp_.append(QRegExp("<blod>.*</blod>"));

    QTextCharFormat italicFormat;
    italicFormat.setFont(italicFont);
    format_.append(italicFormat);
    regexp_.append(QRegExp("<i>.*</i>"));
    format_.append(italicFormat);
    regexp_.append(QRegExp("<italic>.*</italic>"));

    QTextCharFormat codeFormat;
    codeFormat.setForeground(QColor(qSmkApp()->option("color.code")));
    format_.append(codeFormat);
    regexp_.append(QRegExp("^    .*"));

    QTextCharFormat linkFormat;
    linkFormat.setForeground(QColor(qSmkApp()->option("color.link")));
    format_.append(linkFormat);
    regexp_.append(QRegExp("!?\\[.*\\]\\(.*\\)"));

    QTextCharFormat htmlFormat;
    htmlFormat.setForeground(QColor(qSmkApp()->option("color.html")));
    format_.append(htmlFormat);
    regexp_.append(QRegExp("<[0-z,~!@#$%^&*()-+|{}|:;\"'<,>.?/\t ]+>"));

    QTextCharFormat headerFormat;
    headerFormat.setForeground(QColor(qSmkApp()->option("color.header")));
    format_.append(headerFormat);
    regexp_.append(QRegExp("^#.*"));
}

void SmkMarkHighlighter::_aux_multiBlockMatch(
        const QString&         text,
        const QString&         bgnFlag,
        const QString&         endFlag,
        const QTextCharFormat& format,
        const unsigned         field )
{
    if(previousBlockState() == -1)
        return;

    setCurrentBlockState(currentBlockState() & (~field));
    if(previousBlockState() & field) {
        int endid = text.indexOf(endFlag, 0);
        if(endid != -1) {
            // we find a endFlag
            setFormat(0, endid+endFlag.length(), format);
        } else {
            // we do not find a endFlag, mark this line and go on
            setFormat(0, text.length(), format);
            // if(previousBlockState() != -1)
            setCurrentBlockState(currentBlockState() | field);
        }
    }
    else {
        int bgnid = text.indexOf(bgnFlag, 0);
        while(bgnid != -1) {
            int endid = text.indexOf(endFlag, bgnid+bgnFlag.length());
            if(endid != -1) {
                // we find a endFlag
                setFormat(bgnid, endid+endFlag.length()-bgnid, format);
                bgnid = text.indexOf(bgnFlag, endid+endFlag.length());
            } else {
                // we do not find a endFlag, mark this line and go on
                setFormat(bgnid, text.length()-bgnid, format);
                setCurrentBlockState(currentBlockState() | field);
                break;
            }
        }//while(...
    }//if..else...
}

void SmkMarkHighlighter::highlightBlock(const QString& text)
{
    // highlight with regrxp
    for(int i=0; i<format_.size(); i++) {
        int index = regexp_[i].indexIn(text);
        while(index >= 0) {
            int length = regexp_[i].matchedLength();
            this->setFormat(index, length, format_[i]);
            index = regexp_[i].indexIn(text, index + length);
        }
    }

    // code block and latex block highlight
    if(currentBlockState() == -1)
        setCurrentBlockState(0);

    QTextCharFormat codeFormat;
    codeFormat.setForeground(QColor(qSmkApp()->option("color.code")));
    _aux_multiBlockMatch(text, "`", "`", codeFormat, 0x0001);
    // _aux_multiBlockMatch(text, "``", "``", codeFormat, 0x0002);
    // _aux_multiBlockMatch(text, "```", "```", codeFormat, 0x004);

    QTextCharFormat latexFormat;
    latexFormat.setForeground(QColor(qSmkApp()->option("color.latex")));
    _aux_multiBlockMatch(text, "${", "}$",  latexFormat, 0x1000);
    _aux_multiBlockMatch(text, "$$", "$$", latexFormat, 0x2000);
}
