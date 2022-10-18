#ifndef CHILDDOCUMENT_H
#define CHILDDOCUMENT_H

#include <QTextEdit>

class ChildDocument : public QTextEdit
{
    Q_OBJECT

public:
    ChildDocument();

    void createDocument();
    QString getDocumentName();

private:
    QString m_path;
    bool m_saved;
private slots:
    void on_document_changed();

};

#endif // CHILDDOCUMENT_H
