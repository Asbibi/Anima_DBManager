#include "astructure.h"

#include<QDebug>

AStructure::AStructure(const AttributeParam& _sharedParam) :
    AStructure(_sharedParam, nullptr)
{}
AStructure::AStructure(const AttributeParam& _sharedParam, const Structure* _value) :
    Attribute(_sharedParam),
    myValue(nullptr)
{
    Q_ASSERT(mySharedParam.templateStruct != nullptr);
    if (_value == nullptr)
    {
        myValue = new Structure(*mySharedParam.templateStruct);
    }
    else
    {
        Q_ASSERT(mySharedParam.templateStruct == &_value->GetTemplate());
        myValue = new Structure(*_value, true);
    }
}



Attribute* AStructure::CreateDuplica() const
{
    return new AStructure(mySharedParam, myValue);
}
QString AStructure::GetDisplayedText(bool complete) const
{
    return complete ? GetDisplayTextFromAttributes(myValue->GetAttributes()) : "{STRUCT}";
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
    QList<QString> finalList = QList<QString>();
    QList<bool> openBrackets = QList<bool>();
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
    if ((int)finalList.size() != myValue->GetAttributeCount())
    {
        qFatal("\n\nNot a text value per attribute while setting <STRUCTURE> Attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }


    // Apply all the strings in the finalList to their attribute
    for(int i =0; i < (int)finalList.size(); i++)
        myValue->SetAttributeValueFromText(i, finalList[i]);
}
void AStructure::CopyValueFromOther(const Attribute* _other)
{
    const AStructure* other_AS = dynamic_cast<const AStructure*>(_other);
    if (!other_AS)
        return;

    myValue = other_AS->myValue;
}


QList<QString> AStructure::GetDisplayedTexts() const
{
    int _count = myValue->GetAttributeCount();
    QList<QString> strings = QList<QString>();
    strings.reserve(_count);
    for (int i = 0; i< _count; i++)
        strings.push_back(myValue->GetAttribute(i)->GetDisplayedText());

    return strings;
}
const QList<Attribute*>& AStructure::GetAttributes() const
{
    Q_ASSERT(myValue != nullptr);
    return myValue->GetAttributes();
}
QString AStructure::GetDisplayTextFromAttributes(const QList<Attribute*>& _attributes)
{
    QString _text = "{";
    const int attrCount = _attributes.count();
    for (int i = 0; i < attrCount; i++)
    {
        _text += _attributes[i]->GetDisplayedText(true) + ',';
    }
    _text.replace(_text.length() -1, 1, '}');
    return _text;
}
