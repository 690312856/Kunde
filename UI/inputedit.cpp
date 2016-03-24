#include "inputedit.h"

InputEdit::InputEdit(QObject *obj, QObject *parent) :
    QObject(parent)
  , edit(NULL)
  , box(NULL)
{
    if(obj->inherits("QLineEdit"))
        edit = qobject_cast<QLineEdit*>(obj);

    if(obj->inherits("QComboBox"))
        box = qobject_cast<QComboBox*>(obj);

    if(edit)
        connect(edit, SIGNAL(textChanged(QString)),
                this, SLOT(checkTextChanged(QString)));
    if(box)
        connect(box, SIGNAL(currentTextChanged(QString)),
                this, SIGNAL(textChanged(QString)));

    if(edit && edit->toolTip().isEmpty()) edit->setToolTip(edit->objectName());
    if(box && box->toolTip().isEmpty()) box->setToolTip(box->objectName());
}

void InputEdit::clear()
{
    if(edit)
        edit->clear();
}

QString InputEdit::text()
{
    if(edit)
        return edit->text();

    if(box)
        return box->currentText();

    return QString("Hehe");
}

QString InputEdit::name()
{
    if(edit) return edit->toolTip();
    if(box) return box->toolTip();
    return QString();
}

void InputEdit::setText(QString text)
{
    if(edit) edit->setText(text);
    if(box) box->setCurrentText(text);
    emit textChanged(text);
}

void InputEdit::setReadOnly(bool val)
{
    if(edit) edit->setReadOnly(val);
    if(box) box->setDisabled(val);
}

void InputEdit::setDisabled(bool val)
{
    if(edit) edit->setDisabled(val);
    if(box) box->setDisabled(val);
}

bool isNumber(QString & num)
{
    bool ok;
    num.toDouble(&ok);
    return (num.isEmpty() || ok || num == "." || num == "+" || num == "-");
}

void InputEdit::checkTextChanged(QString text)
{
    if(!edit) return;

    if(edit->inputMethodHints() & Qt::ImhPreferNumbers)
    {
        if(isNumber(text))
        {
            emit textChanged(text);
        }
        else
        {
            edit->clear();
            QMessageBox::information(NULL, str("提示"), str("请输入数字\n")
                                     + QString("%0").arg(this->name())
                                     + str("框中请填写数字"));
        }
    }
    else
    {
        emit textChanged(text);
    }
}
