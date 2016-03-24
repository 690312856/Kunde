#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include "global.h"

// qt widgets
#include <QWidget>
#include <QMessageBox>
#include <QObject>
#include <QLineEdit>
#include <QComboBox>

class SHARED_EXPORT InputEdit : public QObject
{
    Q_OBJECT
public:
    InputEdit(QObject* obj, QObject *parent);

    void clear();
    QString text();
    QString name();
    void setText(QString);
    void setReadOnly(bool);
    void setDisabled(bool);

signals:
    void textChanged(QString);

public slots:
    void checkTextChanged(QString);

private:
    QLineEdit * edit;
    QComboBox * box;
};

#endif // INPUTEDIT_H
