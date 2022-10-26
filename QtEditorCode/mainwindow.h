#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "childdocument.h"
#include <QSignalMapper>
#include <QWidget>
#include <QString>
#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menu_docitem_add();

    void on_createAction_triggered();
    void on_document_activated();
    void on_closeAction_triggered();
    void on_closeAllAction_triggered();
    void on_tileAction_triggered();

    void on_cascadeAction_triggered();

    void on_nextAction_triggered();

    void on_previousAction_triggered();
    void on_document_set(QWidget *);

    void on_openAction_triggered();

    void on_saveAction_triggered();

    void on_saveAllAction_triggered();

    void on_undoAction_triggered();

    void on_redoAction_triggered();

    void on_cutAction_triggered();

    void on_copyAction_triggered();

    void on_pasteAction_triggered();

    void on_boldAction_triggered();

    void on_italicAction_triggered();

    void on_underLineAction_triggered();

    void on_fontComboBox_textActivated(const QString &arg1);

    void on_fontSizeComboBox_textActivated(const QString &arg1);

    void on_alignLeftAction_triggered();

    void on_alignCenterAction_triggered();

    void on_alignRightAction_triggered();

    void on_alignJustifyAction_triggered();

    void on_colorAction_triggered();

    void on_printAction_triggered();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    void initMainWindow();
    void formatEnable();
    void documentCreate();
    void documentOpen();
    void documentSave();
    void documentSaveAs();
    void documentPrint();
    void cocumentPrintPreview();

    void documentCopy();
    void documentCut();
    void documentPaste();

    void documentUndo();
    void documentRedo();

    void textBold();
    void textItalic();
    void textUnderline();

    void textFamily(const QString &family);
    void textSize(const QString & size);
    void textColor();

    QMdiSubWindow* documentFind(QString filename);

    ChildDocument * activeDocument();
    QSignalMapper * m_sigmapper;
};
#endif // MAINWINDOW_H
