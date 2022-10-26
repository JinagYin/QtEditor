#ifndef CHILDDOCUMENT_H
#define CHILDDOCUMENT_H

#include <QTextEdit>
#include <QString>
#include <QCloseEvent>

class ChildDocument : public QTextEdit
{
    Q_OBJECT

public:
    ChildDocument();

    void documentCreate();
    bool documentLoad(QString filename);
    bool documentSave();
    bool documentSaveAs();
    bool documentSaveAll();
    bool documentSave(QString filename);

    void setFormatOnSelected(const QTextCharFormat &fmt);
    void documentAlign(int type);

    QString getDocumentName();
    void setPath(const QString path);
protected:
    void closeEvent(QCloseEvent * event);
private:
    QString m_path;
    bool m_saved;

    bool filenameCheck(QString filename);
    bool promptSave();
private slots:
    void on_document_changed();

};

#endif // CHILDDOCUMENT_H
