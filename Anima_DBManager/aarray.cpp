#include "aarray.h"
#include <QDebug>


AArray::AArray(const AttributeParam* _sharedParam) :
    AArray(_sharedParam, nullptr)
{}
AArray::AArray(const AttributeParam* _sharedParam, const std::vector<Attribute*>* _values) :
    Attribute(_sharedParam)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <ARRAY> Attribute:\n\n\t===== Not allowed =====\n\n");
    else if (_sharedParam->templateAtt == nullptr)
        qFatal("\n\nNull Template Attribute given when instancing <ARRAY> Attribute:\n\n\t===== Not allowed =====\n\n");
    else if (_values != nullptr && _values->size() != 0)
    {
        values.reserve(_values->size());
        for(const auto& _val : *_values)
        {
            values.push_back(_val->CreateDuplica());
        }
    }
}
AArray::~AArray()
{
    for(const auto& val : values)
        delete(val);
}


Attribute* AArray::CreateDuplica() const
{
    return new AArray(sharedParam, &values);
}
QString AArray::GetDisplayedText() const
{
    return "[ARRAY]";
}
void AArray::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    for (int i = 0; i< values.size(); i++)
    {
        if (i > 0)
            file << ",";
        values[i]->WriteValue_CSV(file);
    }

    file << ")";
}
std::vector<QString> AArray::GetDisplayedTexts() const
{
    int _count = (int)values.size();
    std::vector<QString> strings = std::vector<QString>();
    strings.reserve(_count);

    for (int i = 0; i< _count; i++)
        strings.push_back(values[i]->GetDisplayedText());

    return strings;
}

void AArray::AddRow()
{
    values.push_back(sharedParam->templateAtt->CreateDuplica());
}
void AArray::RemoveRow(int _index)
{
    if (_index < -1 || _index >= values.size())
        return;

    Attribute* removed = values[_index];
    values.erase(values.begin() + _index);
    delete(removed);
}
