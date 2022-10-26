#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <childdocument.h>
#include <QMdiArea>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QActionGroup>
#include <QColorDialog>
#include <QColor>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    QFontDatabase fontdb;
    foreach(int fontSize, fontdb.standardSizes()) {
        ui->fontSizeComboBox->addItem(QString::number(fontSize));
    }

    QFont font = QApplication::font();
    QString fontSize = QString::number(font.pointSize());
    int fontSizeIdx = ui->fontSizeComboBox->findText(fontSize);
    ui->fontSizeComboBox->setCurrentIndex(fontSizeIdx);
    //设置多文档区域滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    on_document_activated();
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::on_document_activated);
    on_menu_docitem_add();
    connect(ui->menu_W, &QMenu::aboutToShow, this, &MainWindow::on_menu_docitem_add);

    m_sigmapper = new QSignalMapper(this);
    connect(m_sigmapper, SIGNAL(mapped(QWidget *)), this, SLOT(on_document_set(QWidget *)));

    QActionGroup * alignGroup = new QActionGroup(this);
    alignGroup->addAction(this->ui->alignLeftAction);
    alignGroup->addAction(this->ui->alignRightAction);
    alignGroup->addAction(this->ui->alignCenterAction);
    alignGroup->addAction(this->ui->alignJustifyAction);
}

void MainWindow::formatEnable()
{
    ui->boldAction->setEnabled(true);
    ui->italicAction->setEnabled(true);
    ui->underLineAction->setEnabled(true);
    ui->alignLeftAction->setEnabled(true);
    ui->alignRightAction->setEnabled(true);
    ui->alignCenterAction->setEnabled(true);
    ui->alignJustifyAction->setEnabled(true);
    ui->colorAction->setEnabled(true);
}

ChildDocument *MainWindow::activeDocument()
{
    ChildDocument * doc = NULL;
    QMdiSubWindow * win = ui->mdiArea->activeSubWindow();

    if (win != NULL) {
        doc = qobject_cast<ChildDocument *>(win->widget());
    }

    return doc;
}

void MainWindow::documentCreate()
{
    ChildDocument * doc = new ChildDocument();
    ui->mdiArea->addSubWindow(doc);
    connect(doc, SIGNAL(copyAvailable(bool)), ui->cutAction, SLOT(setEnabled(bool)));
    connect(doc, SIGNAL(copyAvailable(bool)), ui->copyAction, SLOT(setEnabled(bool)));
    doc->documentCreate();
    doc->show();
    formatEnable();
}

void MainWindow::documentOpen() {
    QString filename = QFileDialog::getOpenFileName(this, "打开文档", "", "文本文件(*.txt);;"
                                                                         "HTML文件(*.html *.htm);;"
                                                                         "所有文件(*.*)");
    if (filename == NULL || filename.isEmpty()) {
        return;
    }

    QMdiSubWindow * win = documentFind(filename);
    if (win != NULL) {
        ui->mdiArea->setActiveSubWindow(win);
        return;
    }

    ChildDocument * doc = new ChildDocument();
    if (!doc->documentLoad(filename)) {
        qDebug() << "documentLoad() failed!";
        doc->close();
        return;
    }

    ui->mdiArea->addSubWindow(doc);
    connect(doc, SIGNAL(copyAvailable(bool)), ui->cutAction, SLOT(setEnabled(bool)));
    connect(doc, SIGNAL(copyAvailable(bool)), ui->copyAction, SLOT(setEnabled(bool)));
    doc->show();
    formatEnable();
}

void MainWindow::documentSave()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentSave();
    }
}

void MainWindow::documentSaveAs()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentSaveAs();
    }
}

void MainWindow::documentPrint()
{
    ChildDocument * doc = activeDocument();
    if (doc == NULL) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog * dialog = new QPrintDialog(&printer, this);
    dialog->setOption(QAbstractPrintDialog::PrintSelection);
    dialog->setWindowTitle("打印文档");
    if (dialog->exec() == QDialog::Accepted) {
        doc->print(&printer);
    }
    delete dialog;
}

void MainWindow::cocumentPrintPreview()
{

}

void MainWindow::documentCopy()
{
    if (activeDocument()) {
        activeDocument()->copy();
    }
}

void MainWindow::documentCut()
{
    if (activeDocument()) {
        activeDocument()->cut();
    }
}

void MainWindow::documentPaste()
{
    qDebug() << "MainWindow::documentPaste()";
    if (activeDocument()) {
        activeDocument()->paste();
    }
}

void MainWindow::documentUndo()
{
    if (activeDocument()) {
        activeDocument()->undo();
    }
}

void MainWindow::documentRedo()
{
    if (activeDocument()) {
        activeDocument()->redo();
    }
}

void MainWindow::textBold()
{
    QTextCharFormat fmt;
    bool selected = ui->boldAction->isChecked();
    QFont::Weight weight = selected ? QFont::Bold : QFont::Normal;
    fmt.setFontWeight(weight);
    ChildDocument * doc = activeDocument();
    if (doc) {
        doc->setFormatOnSelected(fmt);
    }
}

void MainWindow::textItalic()
{
    qDebug() << "textItalic()  >> ";
    QTextCharFormat fmt;
    bool selected = ui->italicAction->isChecked();
    fmt.setFontItalic(selected);
    ChildDocument * doc = activeDocument();
    if (doc) {
        doc->setFormatOnSelected(fmt);
    }
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    bool selected = ui->underLineAction->isChecked();
    fmt.setFontUnderline(selected);
    ChildDocument * doc = activeDocument();
    if (doc) {
        doc->setFormatOnSelected(fmt);
    }
}

void MainWindow::textFamily(const QString &family)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(family);
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->setFormatOnSelected(fmt);
    }
}

void MainWindow::textSize(const QString &size)
{
    qreal pointSize = size.toDouble();

    if (pointSize <= 0) {
        return;
    }

    QTextCharFormat fmt;
    fmt.setFontPointSize(pointSize);
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->setFormatOnSelected(fmt);
    }
}

void MainWindow::textColor()
{
    ChildDocument * doc = activeDocument();
    if (doc == NULL) {
        return;
    }

    QColor color = QColorDialog::getColor();
    if (!color.isValid()) {
        return;
    }
    QTextCharFormat fmt;
    fmt.setForeground(color);
    doc->setFormatOnSelected(fmt);

    QPixmap pix(16, 16);
    pix.fill(color);
    ui->colorAction->setIcon(pix);
}

QMdiSubWindow* MainWindow::documentFind(QString filename) {
    QString path = QFileInfo(filename).canonicalFilePath();
    QMdiSubWindow * win = NULL;

    foreach(QMdiSubWindow * sub, ui->mdiArea->subWindowList()) {
        ChildDocument * doc = qobject_cast<ChildDocument *>(sub->widget());
        qDebug() << "docname:" << doc->getDocumentName();

        if (doc->getDocumentName() == path) {
            win = sub;
            break;
        }
    }

    return win;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    }
    else {
        event->accept();
    }
}

void MainWindow::on_createAction_triggered()
{
    documentCreate();
}

void MainWindow::on_document_activated()
{
    //判断是否有子窗口
    ChildDocument * doc = activeDocument();
    bool has = doc != NULL;
    ui->saveAction->setEnabled(has);
    ui->saveAllAction->setEnabled(has);
    ui->printAction->setEnabled(has);
    ui->printPreviewAction->setEnabled(has);
    ui->pasteAction->setEnabled(has);
    ui->closeAction->setEnabled(has);
    ui->closeAllAction->setEnabled(has);
    ui->tileAction->setEnabled(has);
    ui->cascadeAction->setEnabled(has);
    ui->nextAction->setEnabled(has);
    ui->previousAction->setEnabled(has);

    //判断是否有内容选中
    bool selected = has && doc->textCursor().hasSelection();
    ui->cutAction->setEnabled(selected);
    ui->copyAction->setEnabled(selected);
    ui->boldAction->setEnabled(selected);
    ui->italicAction->setEnabled(selected);
    ui->underLineAction->setEnabled(selected);
}

void MainWindow::on_menu_docitem_add()
{
    ui->menu_W->clear();
    ui->menu_W->addAction(ui->closeAction);
    ui->menu_W->addAction(ui->closeAllAction);
    ui->menu_W->addAction(ui->tileAction);
    ui->menu_W->addAction(ui->cascadeAction);
    ui->menu_W->addAction(ui->nextAction);
    ui->menu_W->addAction(ui->previousAction);

    QList<QMdiSubWindow *> list = ui->mdiArea->subWindowList();
    if (list.isEmpty()) {
        return;
    }
    ui->menu_W->addSeparator();
    for (int i = 0; i<list.size(); i++) {
        ChildDocument * doc = qobject_cast<ChildDocument *>(list.at(i)->widget());
        QString name = QString("%1, %2").arg(i+1).arg(doc->getDocumentName());
        QAction * action = ui->menu_W->addAction(name);
        action->setCheckable(true);
        action->setChecked(doc == activeDocument());

        connect(action, SIGNAL(triggered(bool)), m_sigmapper, SLOT(map()));
        m_sigmapper->setMapping(action, list.at(i));
    }
    formatEnable();
}

void MainWindow::on_closeAction_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}


void MainWindow::on_closeAllAction_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}


void MainWindow::on_tileAction_triggered()
{
    ui->mdiArea->tileSubWindows();
}


void MainWindow::on_cascadeAction_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}


void MainWindow::on_nextAction_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_previousAction_triggered()
{
    ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow:: on_document_set(QWidget * doc)
{
    if (doc != NULL) {
        ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(doc));
    }
}

void MainWindow::on_openAction_triggered()
{
    documentOpen();
}


void MainWindow::on_saveAction_triggered()
{
    documentSave();
}


void MainWindow::on_saveAllAction_triggered()
{
    documentSaveAs();
}


void MainWindow::on_undoAction_triggered()
{
    documentUndo();
}


void MainWindow::on_redoAction_triggered()
{
    documentRedo();
}


void MainWindow::on_cutAction_triggered()
{
    documentCut();
}


void MainWindow::on_copyAction_triggered()
{
    documentCopy();
}


void MainWindow::on_pasteAction_triggered()
{
    documentPaste();
}


void MainWindow::on_boldAction_triggered()
{
    textBold();
}


void MainWindow::on_italicAction_triggered()
{
    textItalic();
}


void MainWindow::on_underLineAction_triggered()
{
    textUnderline();
}


void MainWindow::on_fontComboBox_textActivated(const QString &family)
{
    textFamily(family);
}


void MainWindow::on_fontSizeComboBox_textActivated(const QString &size)
{
    textSize(size);
}

void MainWindow::on_alignLeftAction_triggered()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentAlign(1);
    }
}


void MainWindow::on_alignCenterAction_triggered()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentAlign(3);
    }
}


void MainWindow::on_alignRightAction_triggered()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentAlign(2);
    }
}


void MainWindow::on_alignJustifyAction_triggered()
{
    ChildDocument * doc = activeDocument();
    if (doc != NULL) {
        doc->documentAlign(4);
    }
}


void MainWindow::on_colorAction_triggered()
{
    textColor();
}


void MainWindow::on_printAction_triggered()
{
    documentPrint();
}

