#include "aarray.h"
#include <QDebug>

AArray::AArray(const AttributeParam& _sharedParam) :
    AArray(_sharedParam, nullptr)
{
     if (_sharedParam.templateAtt == nullptr)
        qFatal("\n\nNull Template Attribute given when instancing <ARRAY> Attribute:\n\n\t===== Not allowed =====\n\n");
}
AArray::AArray(const AttributeParam& _sharedParam, const std::vector<Attribute*>* _values) :
    Attribute(_sharedParam)
{
    if (_sharedParam.templateAtt == nullptr)
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
    return new AArray(mySharedParam, &values);
}
QString AArray::GetDisplayedText(bool complete) const
{
    if (!complete)
        return "[ARRAY]";

    QString _text = "[";
    for (int i = 0; i < (int)(values.size()); i++)
    {
        if (i > 0)
            _text.append(',');
        _text.append(values[i]->GetDisplayedText(true));
    }
    _text.append(']');
    return _text;
}
void AArray::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    for (int i = 0; i < (int)(values.size()); i++)
    {
        if (i > 0)
            file << ",";
        values[i]->WriteValue_CSV(file);
    }

    file << ")";
}
void AArray::SetValueFromText(const QString& text)
{
    // Manage to remove starting '[' and final ']' due to
    QString contentText = text;
    if (contentText[0] != '[')
    {
        qFatal("\n\nMissing start '[' character while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    else if (contentText[contentText.count() - 1] != ']')
    {
        qFatal("\n\nMissing end ']' character while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    contentText = contentText.mid(1, contentText.count() - 2);


    // Read the text
    std::vector<QString> finalList = std::vector<QString>();
    std::vector<bool> openBrackets = std::vector<bool>();
        //  ->  add "true" to indicate an open "{", a false for an open "[", and remove it when closed
    QString currentString = "";

    for (const QChar& chr : contentText)
    {
        int currentBracketCount = (int)openBrackets.size();
        if (chr == ',' && currentBracketCount == 0)
        {
            finalList.push_back(currentString);
            currentString = "";
            continue;
        }

        currentString.append(chr);
        if (chr == '{')
            openBrackets.push_back(true);
        else if (chr == '[')
            openBrackets.push_back(false);
        else if (currentBracketCount != 0)
        {
            if (openBrackets[currentBracketCount - 1] && chr == '}')
                openBrackets.pop_back();
            else if (!openBrackets[currentBracketCount - 1] && chr == ']')
                openBrackets.pop_back();
        }
        else
        {
            if (chr == '}'|| chr == ']')
            {
                qFatal("\n\nA '}' or ']' found with nothing opened while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
                return;
            }
        }
    }
    finalList.push_back(currentString);


    // Checks
    if (openBrackets.size() != 0)
    {
        qFatal("\n\nError in '{' and '[' closing while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    if (finalList.size() != values.size())
    {
        qFatal("\n\nNot a text value per attribute while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }


    // Apply all the strings in the finalList to their attribute
    for(int i =0; i < (int)(finalList.size()); i++)
        values[i]->SetValueFromText(finalList[i]);
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
    values.push_back(mySharedParam.templateAtt->CreateDuplica());
}
void AArray::RemoveRow(int _index)
{
    if (_index < -1 || _index >= (int)(values.size()))
        return;

    Attribute* removed = values[_index];
    values.erase(values.begin() + _index);
    delete(removed);
}
