#include "qastructurelabel.h"

#include "astructure.h"
#include "jsonhelper.h"
#include "qstructuredialog.h"
#include <QJsonDocument>

QAStructureLabel::QAStructureLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}


void QAStructureLabel::SetValue(QJsonObject _value)
{
    myValue = _value;
    setText(JsonHelper::JsonToString(myValue));
}
void QAStructureLabel::SetValue(const QList<Attribute*>& _structureAttrPtr)
{
    myAttributesPtr = _structureAttrPtr;
    SetValue(JsonHelper::ConvertAttributeListToJsonObject(_structureAttrPtr));
}
QJsonObject QAStructureLabel::GetValue() const
{
    return myValue;
}

void QAStructureLabel::EditValue()
{
    auto* dialog = new QStructureDialog(myAttributesPtr, this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        SetValue(dialog->GetValue());
        emit OnValueEdited();
    }
    delete dialog;
}

