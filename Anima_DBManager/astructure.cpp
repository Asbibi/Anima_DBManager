#include "astructure.h"

#include<QDebug>

AStructure::AStructure(const AttributeParam& _sharedParam, TemplateStructure& structureTemplate) :
    Attribute(_sharedParam),
    value(structureTemplate)
{}
AStructure::AStructure(const AttributeParam& _sharedParam, const Structure& _value) :
    Attribute(_sharedParam),
    value(_value)
{}



Attribute* AStructure::CreateDuplica() const
{
    return new AStructure(mySharedParam, value);
}
QString AStructure::GetDisplayedText(bool complete) const
{
    if (!complete)
        return "{STRUCT}";

    QString _text = "{";
    value.GetAttributesDisplayedText(_text);
    _text.append('}');
    return _text;
}
QString AStructure::GetAttributeAsCSV() const
{
    qWarning("TODO");
    //value.WriteValue_CSV(file);
    return "";
}
void AStructure::SetValueFromText(const QString& text)
{
    // Manage to remove starting '[' and final ']'
    QString contentText = text;
    if (contentText[0] != '{')
    {
        qFatal("\n\nMissing start '{' character while setting <STRUCTURE> attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    else if (contentText[contentText.count() - 1] != '}')
    {
        qFatal("\n\nMissing end '}' character while setting <STRUCTURE> attribute's value:\n\n\t===== Abort =====\n\n");
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
                qFatal("\n\nA '}' or ']' found with nothing opened while setting <STRUCTURE> Attribute's value:\n\n\t===== Abort =====\n\n");
                return;
            }
        }
    }
    finalList.push_back(currentString);


    // Checks
    if (openBrackets.size() != 0)
    {
        qFatal("\n\nError in '{' and '[' closing while setting <STRUCTURE> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }
    if ((int)finalList.size() != value.GetAttributeCount())
    {
        qFatal("\n\nNot a text value per attribute while setting <STRUCTURE> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }


    // Apply all the strings in the finalList to their attribute
    for(int i =0; i < (int)finalList.size(); i++)
        value.SetAttributeValueFromText(i, finalList[i]);
}
void AStructure::CopyValueFromOther(const Attribute* _other)
{
    const AStructure* other_AS = dynamic_cast<const AStructure*>(_other);
    if (!other_AS)
        return;

    value = other_AS->value;
}


std::vector<QString> AStructure::GetDisplayedTexts() const
{
    int _count = value.GetAttributeCount();
    std::vector<QString> strings = std::vector<QString>();
    strings.reserve(_count);
    for (int i = 0; i< _count; i++)
        strings.push_back(value.GetAttribute(i)->GetDisplayedText());

    return strings;
}
