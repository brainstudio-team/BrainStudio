#include "xmlwindow.h"

XmlWindow::XmlWindow(QWidget *parent) : QWidget(parent){

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);
    font.setBold(true);

    setNulls();

    //Used to avoid loading simulation again when this caused the initial change
    during_update_of_schema = false;

    textEdit = new CodeEditor(this);
    textEdit->setFont(font);
    textEdit->setWordWrapMode( QTextOption::NoWrap );


    QLabel *aa = new QLabel(this);
    aa->setText("");
    aa->setMinimumHeight(37);
    aa->setMaximumHeight(37);
    toolBar = new QToolBar("File/edit", aa);//addToolBar(tr("File"));

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0,0,0,0));
    layout->addWidget(aa);
    layout->addWidget(textEdit);
    this->setLayout(layout);

    highlighter = new XMLHighlighter(textEdit->document());

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");
    //setUnifiedTitleAndToolBarOnMac(true);
}

XmlWindow::~XmlWindow(){
    delete textEdit;                          // Zaf: Do I need this or does it cause any problems???
}

void XmlWindow::setNulls(){
    fileMenu = NULL;
    editMenu = NULL;
    helpMenu = NULL;
    toolBar = NULL;
    newAct = NULL;
    openAct = NULL;
    saveAct = NULL;
    saveAsAct = NULL;
    exitAct = NULL;
    cutAct = NULL;
    copyAct = NULL;
    pasteAct = NULL;
    aboutAct = NULL;
    aboutQtAct = NULL;
    buildAct = NULL;
}

void XmlWindow::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        writeSettings();
        event->accept();
    }
    else{
        event->ignore();
    }
}

void XmlWindow::newFile(){
    if(maybeSave()){
        textEdit->clear();
        setCurrentFile("");
    }
}

void XmlWindow::open(){
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool XmlWindow::save(){
    if(curFile.isEmpty()){
        return saveAs();
    }
    else{
        this->build_xml();
        return saveFile(curFile);
    }
}

bool XmlWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void XmlWindow::about(){
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void XmlWindow::documentWasModified(){
    if(textEdit->document()->isModified()){
        buildAct->setIcon(QIcon(":/new/prefix1/icons/left.png"));
        setWindowModified(true);
    }
    else{
        buildAct->setIcon(QIcon(":/new/prefix1/icons/left-gray.png"));
        setWindowModified(false);
    }
}

void XmlWindow::build_xml(){
    if(!curFile.isEmpty() && textEdit != NULL && buildAct != NULL &&
            textEdit->document()->isModified()){
        buildAct->setIcon(QIcon(":/new/prefix1/icons/left-gray.png"));
        textEdit->document()->setModified(false);
        setWindowModified(false);

        saveFile(UserData::workspace_path+"/temp.brn"); //TEMP_FILE
        //statusBar()->showMessage(tr("File saved and built"), 2000);

        during_update_of_schema = true;
        emit buildTemp_xmlSignal();
    }
}


void XmlWindow::createActions(){
    /*newAct = new QAction(QIcon(":/new/prefix1/icons/new.png"), tr("&New"),this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/new/prefix1/icons/open.png"), tr("&Open..."),
                          this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/new/prefix1/icons/save.png"), tr("&Save"),
                          this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/new/prefix1/icons/cut.png"), tr("Cu&t"),
                         this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/new/prefix1/icons/copy.png"), tr("&Copy"),
                          this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/new/prefix1/icons/paste.png"), tr("&Paste"),
                           this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));*/

    buildAct = new QAction(QIcon(":/new/prefix1/icons/left.png"), tr("&Build"),
                          this);
    buildAct->setShortcuts(QKeySequence::Open);
    buildAct->setStatusTip(tr("Build this XML file and show the outcome"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(build_xml()));
}

void XmlWindow::createMenus(){
/*    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(buildAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);*/
    //menuBar()->setVisible(false);
}

void XmlWindow::createToolBars(){

    /*toolBar->addAction(newAct);
    toolBar->addAction(openAct);
    toolBar->addAction(saveAct);*/

    toolBar->addSeparator();
    toolBar->addAction(buildAct);

    toolBar->addSeparator();
    /*toolBar->addAction(cutAct);
    toolBar->addAction(copyAct);
    toolBar->addAction(pasteAct);*/
}

void XmlWindow::createStatusBar(){
    //statusBar()->showMessage(tr("Ready"));
}

void XmlWindow::readSettings(){
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void XmlWindow::writeSettings(){
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool XmlWindow::maybeSave(){
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool XmlWindow::loadFile(const QString &fileName){
    if(during_update_of_schema){
        during_update_of_schema = false;
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    //statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool XmlWindow::saveFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
   // statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void XmlWindow::setCurrentFile(const QString &fileName){
    curFile = fileName;
    buildAct->setIcon(QIcon(":/new/prefix1/icons/left-gray.png"));
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString XmlWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}



void XmlWindow::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_F5){
        this->build_xml();
    }
    else if(event->key()==Qt::Key_F3)   emit f3_xmlSignal();
    else if(event->key()==Qt::Key_F4)   emit f4_xmlSignal();
    else if(event->key()==Qt::Key_F6)   emit f6_xmlSignal();
    else if(event->key()==Qt::Key_F7)   emit f7_xmlSignal();
}





















