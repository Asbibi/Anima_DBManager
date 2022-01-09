#include "enumerator.h"

Enumerator::Enumerator(QString _name) : name(_name) {}
Enumerator::Enumerator(QString _name, QString* _values, int _count) : Enumerator(_name, _values, nullptr, _count) {}
Enumerator::Enumerator(QString _name, QString* _values, QColor* _colors, int _count) : Enumerator(_name)
{
    if (_values == nullptr)
        return;

    values.reserve(_count);
    colors.reserve(_count);
    bool _colorsIsNull = _colors == nullptr;

    for (int i = 0; i < _count; i++)
    {
        values.push_back(_values[i]);
        colors.push_back(_colorsIsNull ? QColor(240,240,240) : _colors[i]);
    }
}
Enumerator::Enumerator(QString _name, std::vector<QString> _values) : name(_name), values(_values)
{
    colors.reserve(values.size());
    for(const auto& val : values)
        colors.push_back(QColor(204,240,240));
}
Enumerator::Enumerator(QString _name, std::vector<QString> _values, std::vector<QColor> _colors) : name(_name), values(_values), colors(_colors) {}





QString Enumerator::GetName() const
{
    return name;
}
int Enumerator::GetValueCount() const
{
    return (int)values.size();
}
QString Enumerator::GetValue(int _index) const
{
    if (_index < 0 || _index >= values.size())
        return "";

    return values[_index];
}
int Enumerator::GetValueIndex(const QString& _value) const
{
    for(int i = 0; i < values.size(); i++)
        if (_value == values[i])
            return i;

    return -1;
}




void Enumerator::AddValue(const QString& _value)
{
    AddValue(_value, QColor(240,240,240));
}
void Enumerator::AddValue(const QString& _value, const QColor& _color)
{
    for(const auto& val : values)
        if(_value == val)
            return;

    values.push_back(_value);
    colors.push_back(_color);
}
void Enumerator::RemoveValue(const QString& _value)
{
    RemoveValue(GetValueIndex(_value));
}
void Enumerator::RemoveValue(int _index)
{
    if (_index < 0 || _index >= values.size())
        return;

    values.erase(values.begin() + _index);
    colors.erase(colors.begin() + _index);
}
