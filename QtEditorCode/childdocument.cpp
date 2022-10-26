#include "childdocument.h"

#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QDebug>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QTextCursor>

ChildDocument::ChildDocument()
{
    //when window close, delete object
    setAttribute(Qt::WA_DeleteOnClose);

}

void ChildDocument::documentCreate()
{
    static int docSeqCnt = 1;
    m_path = QString("新建文档 %1").arg(docSeqCnt++);
    //设置窗体标题
    setWindowTitle(m_path + "[*]");
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(on_document_changed()));
    m_saved = false;
}

bool ChildDocument::documentLoad(const QString filename) {
    qDebug() << "documentLoad(" << filename << ")";

    bool ret = false;

    if (filename == NULL || filename.isEmpty()) {
        return ret;
    }

    QFile file(filename);
    if (!file.exists()) {
        return ret;
    }

    if (!file.open(QFile::ReadOnly)) {
        return ret;
    }

    QByteArray text = file.readAll();
    if (Qt::mightBeRichText(text)) {
        setHtml(text);
    } else {
        setPlainText(text);
    }
    setPath(filename);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(on_document_changed()));
    ret = true;
    return ret;
}

bool ChildDocument::documentSave()
{
    qDebug() << "documentSave() >>";
    bool ret = false;
    if (m_saved) {
        ret = documentSave(m_path);
    } else {
        ret = documentSaveAs();
    }
    return ret;
}

bool ChildDocument::documentSaveAs()
{
    bool ret = false;
    QString filename = QFileDialog::getSaveFileName(this, "另存为", m_path, "HTML文档(*.html *.htm);;"
                                                                            "文本文件(*.txt);;"
                                                                            "所有文件(*.*)");

    if (filename.isEmpty()) {
        return ret;
    }

    ret = documentSave(filename);

    return ret;
}

bool ChildDocument::documentSaveAll()
{
    bool ret = false;
    return ret;
}

bool ChildDocument::documentSave(QString filename)
{
    bool ret = false;

    if (!filenameCheck(filename)) {
        return ret;
    }

    QTextDocumentWriter writer(filename);
    if ((ret = writer.write(this->document()))) {
        setPath(filename);
    }

    return ret;
}

void ChildDocument::setFormatOnSelected(const QTextCharFormat &fmt)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(fmt);
    mergeCurrentCharFormat(fmt);
}

void ChildDocument::documentAlign(int type)
{
    if (type == 1) {
        setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    } else if (type == 2) {
        setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    } else if (type == 3) {
        setAlignment(Qt::AlignCenter);
    } else if (type == 4) {
        setAlignment(Qt::AlignJustify);
    }
}

QString ChildDocument::getDocumentName()
{
    return m_path;
}

bool ChildDocument::filenameCheck(QString filename) {
    return filename.endsWith(".html", Qt::CaseInsensitive) ||
            filename.endsWith(".htm", Qt::CaseInsensitive) ||
            filename.endsWith(".txt", Qt::CaseInsensitive);
}

bool ChildDocument::promptSave()
{
    bool ret = true;
    if (!document()->isModified()) {
        return ret;
    }

    QMessageBox::StandardButton result;
    result = QMessageBox::warning(this, "系统提示", QString("文档 %1 已修改").arg(getDocumentName()), QMessageBox::Yes | QMessageBox::Discard | QMessageBox::No);
    if (result == QMessageBox::No) {
        return ret;
    }

    return documentSave();
}

void ChildDocument::setPath(const QString path)
{
    m_path = QFileInfo(path).canonicalFilePath();
    m_saved = true;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(getDocumentName() + "[*]");
}

void ChildDocument::closeEvent(QCloseEvent *event)
{
    if (promptSave())
    {
        event->accept();
    } else {
        event->ignore();
    }
}

void ChildDocument::on_document_changed()
{
    setWindowModified(document()->isModified());
}


