#include "xmlhighlighter.h"

XMLHighlighter::XMLHighlighter(QTextDocument*parent):QSyntaxHighlighter(parent){
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Normal);
    QStringList keywordPatterns;
    keywordPatterns << "\\b?xml\\b" << "/>" << ">" << "<" << "</" ;
    foreach(const QString &pattern, keywordPatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("<!--[^\n]*-->");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //functionFormat.setFontItalic(true);
    functionFormat.setFontWeight(QFont::Normal);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=[\s/>])");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");


    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::red);
    QStringList classPatterns;
    classPatterns << "<nodes>" << "</nodes>"
                  << "<actions>" << "</actions>"
                  << "<edges>" << "</edges>"
                  << "<textBlocks>" << "</textBlocks>";
    foreach(const QString &pattern, classPatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = classFormat;
        highlightingRules.append(rule);
    }

    elementFormat.setForeground(Qt::darkBlue);
    QStringList elementPatterns;
    elementPatterns << "<stop" << "<adjust" << "<stimulate" << "/>" << "\">"
          << "<Izhikevich-NeMo" << "</Izhikevich-NeMo>"
          << "<RateLayer-Python>" << "</RateLayer-Python>"
          << "<PoissonSource-NeMo" << "</PoissonSource-NeMo>"
          << "<NeMoDirectPathway-NeMo" << "</NeMoDirectPathway-NeMo>"
          << "<NeMoSynapticPathway-NeMo" << "</NeMoSynapticPathway-NeMo>"
          << "<SpikeToRateConverter-Python" << "</SpikeToRateConverter-Python>"
          << "<RateToSpikeConverter-Python" << "</RateToSpikeConverter-Python>"
          << "<action>" << "</action>"
          << "<schema" << "</schema>"
          << "<textBlock>" << "</textBlock>";
    foreach(const QString &pattern, elementPatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = elementFormat;
        highlightingRules.append(rule);
    }
}

void XMLHighlighter::highlightBlock(const QString &text){
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text,
                                                    startIndex + commentLength);
    }
}
