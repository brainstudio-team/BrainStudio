#ifndef XMLWINDOW_H
#define XMLWINDOW_H

#include <QtWidgets>

#include "source/xmleditor/codeeditor.h"
#include "source/xmleditor/xmlhighlighter.h"
#include "source/helper/userdata.h"

class QAction;
class QMenu;
class QPlainTextEdit;

class XmlWindow : public QWidget{
    Q_OBJECT

protected:
    QVBoxLayout *layout;
    QToolBar *fileToolbar;

public:
    XmlWindow(QWidget *parent = 0);
    ~XmlWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent * event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void build_xml();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();

    void setNulls();
public:
    bool loadFile(const QString &fileName);
private:
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    CodeEditor *textEdit;
    XMLHighlighter *highlighter;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *toolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *buildAct;

    bool during_update_of_schema;

signals:
    void build_signal();
    void buildTemp_xmlSignal();
    void f3_xmlSignal();
    void f4_xmlSignal();
    void f6_xmlSignal();
    void f7_xmlSignal();

    //void schemaSetHighlighted(QString index); not used but it might be nice
    void schemaDoubleClick(QPoint point);
};

#endif // XMLWINDOW_H
