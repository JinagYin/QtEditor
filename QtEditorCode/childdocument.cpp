#include "childdocument.h"

#include <QFileInfo>

ChildDocument::ChildDocument()
{
    //when window close, delete object
    setAttribute(Qt::WA_DeleteOnClose);

}

void ChildDocument::createDocument()
{
    static int docSeqCnt = 1;
    m_path = QString("新建文档 %1").arg(docSeqCnt++);
    //设置窗体标题
    setWindowTitle(m_path + "[*]");
    connect(document(), SIGNAL(contentChanged()), this, SLOT(on_document_changed()));

}

QString ChildDocument::getDocumentName()
{
    return QFileInfo(m_path).fileName();
}

void ChildDocument::on_document_changed()
{
    setWindowModified(document()->isModified());

}


