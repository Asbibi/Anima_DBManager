#include "qastructurelabel.h"

#include "astructure.h"
#include "qstructuredialog.h"

QAStructureLabel::QAStructureLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}
QAStructureLabel::~QAStructureLabel()
{
    ClearMyAttributes();
}
void QAStructureLabel::ClearMyAttributes()
{
    for (auto* attr: myAttributesPtr)
    {
        attr->PreManualDelete();
        delete attr;
    }
    myAttributesPtr.clear();
}


void QAStructureLabel::SetValue(const QList<Attribute*>& _structureAttrPtr)
{
    ClearMyAttributes();
    for (const auto* constAttr : _structureAttrPtr)
    {
        myAttributesPtr.push_back(constAttr->CreateDuplica());
    }
    setText(AStructure::GetValueAsTextFromAttributes(myAttributesPtr));
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

