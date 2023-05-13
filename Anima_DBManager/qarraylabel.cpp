#include "qarraylabel.h"

#include "aarray.h"


QArrayLabel::QArrayLabel(QWidget* _parent) :
    QLabel(_parent)
{
    setMinimumWidth(110);
}

void QArrayLabel::SetValue(TemplateAttribute* _template, const QStringList& _attrStringList)
{
    setText(AArray::GetShortDisplayedString(_attrStringList.count()));
}
