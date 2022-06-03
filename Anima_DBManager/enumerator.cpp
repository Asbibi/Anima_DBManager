#include "enumerator.h"

#include <QWidget>

Enumerator::Enumerator(const QString& _name) : name(_name) {}
Enumerator::Enumerator(const QString& _name, const QString* _values, const int _count) : Enumerator(_name, _values, nullptr, _count) {}
Enumerator::Enumerator(const QString& _name, const QString* _values, const QColor* _colors, const int _count) : Enumerator(_name)
{
    if (_values == nullptr)
        return;

    values.reserve(_count);
    colors.reserve(_count);
    bool _colorsIsNull = _colors == nullptr;

    for (int i = 0; i < _count; i++)
    {
        values.push_back(_values[i]);
        if (!_colorsIsNull)
        {
            colors.push_back(_colors[i]);
        }
    }
}
Enumerator::Enumerator(const QString& _name, const std::vector<QString>& _values) :
    name(_name)
{
    values.assign(_values.begin(), _values.end());
}
Enumerator::Enumerator(const QString& _name, const std::vector<QString>& _values, const std::vector<QColor>& _colors) :
    name(_name)
{
    values.assign(_values.begin(), _values.end());
    colors.assign(_colors.begin(), _colors.end());
}
Enumerator::Enumerator(const Enumerator& _another) :
    Enumerator(_another.name, _another.values, _another.colors)
{}


void Enumerator::operator=(const Enumerator& _another)
{
    name = _another.name;
    values.assign(_another.values.begin(), _another.values.end());
    colors.assign(_another.colors.begin(), _another.colors.end());
}


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

void Enumerator::SetColorToWidget(int _index, QWidget* _widget) const
{
    if (_index < 0 || _index >= colors.size())
        return ;

    _widget->setStyleSheet("QComboBox { color: " + colors[_index].name(QColor::HexRgb) + "; }");
    // Notabene: Background Color -> "background-color:..."
}




void Enumerator::AddValue(const QString& _value)
{
    AddValue(_value, nullptr);
}
void Enumerator::AddValue(const QString& _value, const QColor* _color)
{
    for(const auto& val : values)
        if(_value == val)
            return;

    values.push_back(_value);
    // We add a color only if we already colors only
    if (colors.size() > 0)
    {
        colors.push_back(_color ? *_color : QColorConstants::Black);
    }
    // Or if we have a color and it's our first value
    else if (_color && values.size() == 1)
    {
        colors.push_back( *_color);
    }

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
