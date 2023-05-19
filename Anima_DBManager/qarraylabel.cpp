#include "qarraylabel.h"

#include "qarraydialog.h"


QArrayLabel::QArrayLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}

void QArrayLabel::SetValue(const AArray* _arrayAttr)
{
    myArrayAttribute = _arrayAttr;
    setText(myArrayAttribute->GetDisplayedText());
}
QString QArrayLabel::GetValue() const
{
    qFatal("TODO");
    return "";
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
        // a value to change
        //emit OnValueEdited();
        qDebug() << "TODO : export dialog attribute list as QString and emit signal";
    }
    delete dialog;
}
