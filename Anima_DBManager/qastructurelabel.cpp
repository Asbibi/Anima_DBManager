#include "qastructurelabel.h"

#include "astructure.h"
#include "qstructuredialog.h"

QAStructureLabel::QAStructureLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}


void QAStructureLabel::SetValue(const QList<Attribute*>& _structureAttrPtr)
{
    myAttributesPtr = _structureAttrPtr;
    setText(AStructure::GetDisplayTextFromAttributes(myAttributesPtr));
}
QString QAStructureLabel::GetValue() const
{
    return text();
}

void QAStructureLabel::EditValue()
{
    auto* dialog = new QStructureDialog(myAttributesPtr, this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        setText(dialog->GetValue());
        emit OnValueEdited();
    }
    delete dialog;
}

