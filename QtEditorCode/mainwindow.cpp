#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <childdocument.h>

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

}

void MainWindow::createDocument()
{
    ChildDocument * doc = new ChildDocument();
    ui->mdiArea->addSubWindow(doc);
    connect(doc, SIGNAL(copyAvailable(bool)), ui->cutAction, SLOT(setEnable(bool)));
    connect(doc, SIGNAL(copyAvailable(bool)), ui->copyAction, SLOT(setEnable(bool)));
    doc->createDocument();
    doc->show();
    formatEnable();
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

void MainWindow::on_createAction_triggered()
{
    createDocument();
}

