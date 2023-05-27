#include "qarraylabel.h"

#include "qarraydialog.h"


QArrayLabel::QArrayLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}

void QArrayLabel::ComputeMyValue(const QList<Attribute*>& _attributes)
{
    myValue = "[";
    for (const auto* attr : _attributes)
    {
        myValue += attr->GetDisplayedText(true);
        myValue += ',';
    }
    const int strLenght = myValue.count();
    if (strLenght > 1)
    {
        myValue[strLenght-1] = ']';
    }
    else
    {
        myValue += ']';
    }
}
void QArrayLabel::SetValue(const AArray* _arrayAttr)
{
    myArrayAttribute = _arrayAttr;
    setText(myArrayAttribute->GetDisplayedText());
    ComputeMyValue(myArrayAttribute->GetAttributes());
}
QString QArrayLabel::GetValue() const
{
    return myValue;
}

void QArrayLabel::EditValue()
{
    auto* dialog = new QArrayDialog(myArrayAttribute->GetArrayElementTemplate(),
                                    myArrayAttribute->GetAttributes(),
                                    this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {        
        ComputeMyValue(dialog->GetAttributes());
        emit OnValueEdited();
        setText(myArrayAttribute->GetDisplayedText());
    }
    delete dialog;
}
