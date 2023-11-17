#include "qarraylabel.h"

#include "jsonhelper.h"
#include "qarraydialog.h"
#include <QJsonDocument>

QArrayLabel::QArrayLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}

void QArrayLabel::ComputeMyValue(const QList<Attribute*>& _attributes)
{
    myValue = JsonHelper::ConvertAttributeListToJsonArray(_attributes);
    setText(JsonHelper::JsonToString(myValue));
}
void QArrayLabel::SetValue(const AArray* _arrayAttr)
{
    myArrayAttribute = _arrayAttr;
    //setText(myArrayAttribute->GetDisplayedText());
    ComputeMyValue(myArrayAttribute->GetAttributes());
}
QJsonArray QArrayLabel::GetValue() const
{
    return myValue;
}

void QArrayLabel::EditValue()
{
    const auto& param = myArrayAttribute->GetTemplateParam();
    auto* dialog = new QArrayDialog(myArrayAttribute->GetArrayElementTemplate(),
                                    myArrayAttribute->GetAttributes(),
                                    !param.ignoreMin,
                                    param.min_i,
                                    !param.ignoreMax,
                                    param.max_i,
                                    this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {        
        ComputeMyValue(dialog->GetAttributes());
        emit OnValueEdited();
        //setText(myArrayAttribute->GetDisplayedText());
    }
    delete dialog;
}
