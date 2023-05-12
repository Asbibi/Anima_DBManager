#include "aarray.h"

#include "templateattribute.h"
#include <QDebug>

AArray::AArray(const AttributeParam& _sharedParam) :
    Attribute(_sharedParam)
{
     if (_sharedParam.templateAtt == nullptr)
        qFatal("\n\nNull Template Attribute given when instancing <ARRAY> Attribute:\n\n\t===== Not allowed =====\n\n");
}
AArray::AArray(const AttributeParam& _sharedParam, const QList<Attribute*>& _values) :
    AArray(_sharedParam)
{    
    myValues.reserve(_values.count());
    for(const auto* _val : _values)
    {
        myValues.push_back(_val->CreateDuplica());
    }
}
AArray::~AArray()
{
    for(const auto& val : myValues)
        delete(val);
}


Attribute* AArray::CreateDuplica() const
{
    return new AArray(mySharedParam, myValues);
}
QString AArray::GetDisplayedText(bool complete) const
{
    if (!complete)
        return "[ARRAY]";

    QString _text = "[";
    for (int i = 0; i < (int)(myValues.size()); i++)
    {
        if (i > 0)
            _text.append(',');
        _text.append(myValues[i]->GetDisplayedText(true));
    }
    _text.append(']');
    return _text;
}
void AArray::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    for (int i = 0; i < (int)(myValues.size()); i++)
    {
        if (i > 0)
            file << ",";
        myValues[i]->WriteValue_CSV(file);
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
    if (finalList.size() != myValues.size())
    {
        qFatal("\n\nNot a text value per attribute while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }


    // Apply all the strings in the finalList to their attribute
    for(int i =0; i < (int)(finalList.size()); i++)
        myValues[i]->SetValueFromText(finalList[i]);
}
void AArray::CopyValueFromOther(const Attribute* _other)
{
    const AArray* other_AA = dynamic_cast<const AArray*>(_other);
    if (!other_AA || mySharedParam.templateAtt != other_AA->mySharedParam.templateAtt)
        return;

    while (myValues.size() > 0)
        RemoveRow(0);

    for (const auto* attr : other_AA->myValues)
        myValues.push_back(attr->CreateDuplica());
}
void AArray::ReadValue_CSV(const QString& text)
{
    qFatal("TODO");
}



QStringList AArray::GetDisplayedTexts() const
{
    int count = (int)myValues.size();
    QStringList strings = QStringList();
    strings.reserve(count);

    for (const auto* val : myValues)
    {
        strings.push_back(val->GetDisplayedText());
    }

    return strings;
}
void AArray::AddRow()
{
    myValues.push_back(mySharedParam.templateAtt->GenerateAttribute());
}
void AArray::RemoveRow(int _index)
{
    if (_index < 0 || _index >= myValues.count())
        return;

    Attribute* removed = myValues.takeAt(_index);
    delete(removed);
}
