#include "attribute.h"
#include "qattribute.h"


Attribute::Attribute(const AttributeParam* _sharedParam) :
    sharedParam(_sharedParam)
{}

Attribute::~Attribute()
{}

void Attribute::EmitValueChanged()
{
    emit OnValueChanged(this);
}
