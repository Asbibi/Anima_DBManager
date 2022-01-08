#include "enumerator.h"

Enumerator::Enumerator() {}

int Enumerator::GetValueNumber() const
{
    return values.size();
}
QString Enumerator::GetValue(int _index) const
{
    if (_index < 0 || _index >= values.size())
        return "";

    return values[_index];
}

void Enumerator::AddValue(QString _value)
{
    AddValue(_value, QColor(240,240,240));
}
void Enumerator::AddValue(QString _value, QColor _color)
{
    for(const auto& val : values)
        if(_value == val)
            return;

    values.push_back(_value);
    colors.push_back(_color);
}
