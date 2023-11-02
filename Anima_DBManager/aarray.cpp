#include "aarray.h"

#include "templateattribute.h"
#include <QDebug>
#include <QJsonArray>

AArray::AArray(TemplateAttribute& _template) :
    Attribute(_template)
{
    Q_ASSERT(myTemplate.GetSharedParam().templateAtt != nullptr);
}
AArray::AArray(TemplateAttribute& _template, const QList<Attribute*>& _values) :
    AArray(_template)
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
    {
        val->PreManualDelete();
        delete(val);
    }
}



void AArray::SetCount(int _count)
{
    const int currentSize = myValues.size();
    if (currentSize < _count)
    {
        const int diff = _count - currentSize;
        for (int i = 0; i < diff; i++)
        {
            AddRow(-1);
        }
    }
    else if (currentSize > _count)
    {
        const int diff = currentSize - _count;
        for (int i = 0; i < diff; i++)
        {
            RemoveRow(0);
        }
    }
    Q_ASSERT(_count == myValues.size());
}

QString AArray::GetDisplayedText() const
{
    return GetShortDisplayedString(myValues.count());
}
QString AArray::GetValueAsText() const
{
    return GetStructureStringFromList(GetValuesAsTexts());
}
QString AArray::GetAttributeAsCSV() const
{
    const bool shouldWrapInQuote = AttributeTypeHelper::ShouldBeWrappedInQuoteInCSV(myTemplate.GetSharedParam().templateAtt->GetType());
    QString arrayAsCSV = "(";

    for (int i = 0; i < (int)(myValues.size()); i++)
    {
        if (i > 0)
        {
            arrayAsCSV.append(',');
        }

        if (shouldWrapInQuote)
        {
            arrayAsCSV.append(QString(AttributeTypeHelper::csvDoubleQuoteWrapper).arg(myValues[i]->GetAttributeAsCSV()));
        }
        else
        {
            arrayAsCSV.append(myValues[i]->GetAttributeAsCSV());
        }
    }

    return arrayAsCSV.append(')');
}
QJsonValue AArray::GetAttributeAsJSON() const
{
    QJsonArray attributesAsJSON = QJsonArray();

    for (const auto* attr : myValues)
    {
        attributesAsJSON.append(attr->GetAttributeAsJSON());
    }

    return QJsonValue(attributesAsJSON);
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
    QList<QString> finalList = QList<QString>();
    if (!contentText.isEmpty())
    {
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
                    qFatal("\n\nA '}' or ']' found with nothing opened while setting <ARRAY> Attribute's value:\n\n\t===== Abort =====\n\n");
                    return;
                }
            }
        }
        finalList.push_back(currentString);
        Q_ASSERT(openBrackets.size() == 0);
    }

    SetCount(finalList.size());


    // Apply all the strings in the finalList to their attribute
    for(int i =0; i < (int)(finalList.size()); i++)
        myValues[i]->SetValueFromText(finalList[i]);
}
void AArray::CopyValueFromOther(const Attribute* _other)
{
    const AArray* other_AA = dynamic_cast<const AArray*>(_other);
    if (!other_AA)
        return;

    if (myTemplate.GetSharedParam().templateAtt != other_AA->myTemplate.GetSharedParam().templateAtt)
    {
        SetValueFromText(other_AA->GetValueAsText());
        return;
    }

    const int otherCount = other_AA->myValues.size();
    while (myValues.size() > otherCount)
        RemoveRow(0);

    const int myCount = myValues.size();
    for (int i=0; i < myCount; i++)
        myValues[i]->CopyValueFromOther(other_AA->myValues[i]);
    for (int i=myCount; i < otherCount; i++)
        myValues.push_back(other_AA->myValues[i]->CreateDuplica());
}
bool AArray::ReadValue_JSON(const QJsonValue& _value)
{
    if (!_value.isArray())
    {
        return false;
    }

    QJsonArray jsonAsArray = _value.toArray();
    const int jsonCount = jsonAsArray.count();
    SetCount(jsonCount);

    for (int i = 0; i < jsonCount; i++)
    {
        myValues[i]->ReadValue_JSON(jsonAsArray[i]);
    }

    return true;
}
void AArray::ReadValue_CSV(const QString& text)
{
    Q_ASSERT(text.length() > 1 && text[0] == '(');
    QString editedText = text.sliced(1, text.length() -2);

    int currentElementIndex = 0;
    int level = 0;
    // NOTE : this piece of code is there to make the short string array able to handle any char in the shortstring,
    //          however this requires the shortstring values to be between " ", which is currently not the case
    //          (but might be if Unreal accepts it in its CSV parsing, need to check later)

    //bool inText = false;
    int start = 0;
    int count = 0;

    const int textLength = editedText.length();
    for (int i = 0; i < textLength; i++)
    {
        const QChar& iChar = editedText[i];
        /*if (iChar == '"')
        {
            inText = !inText;
        }

        if (!inText)
        {*/
            if (iChar == '(')
            {
                level++;
            }
            else if (iChar == ')')
            {
                Q_ASSERT(level > 0);
                level--;
            }
            else if (iChar == ',' && level == 0)
            {
                AddRow(currentElementIndex);
                myValues[currentElementIndex]->ReadValue_CSV(editedText.mid(start, count));
                currentElementIndex++;
                start += count + 1;
                count = 0;
                continue;
            }
        //}

        count++;
    }

    if (count !=0 && level == 0)
    {
        AddRow(currentElementIndex);
        myValues[currentElementIndex]->ReadValue_CSV(editedText.mid(start, count));
    }
}





TemplateAttribute* AArray::GetArrayElementTemplate() const
{
    return myTemplate.GetSharedParam().templateAtt;
}
QStringList AArray::GetValuesAsTexts() const
{
    int count = (int)myValues.size();
    QStringList strings = QStringList();
    strings.reserve(count);

    for (const auto* val : myValues)
    {
        strings.push_back(val->GetValueAsText());
    }

    return strings;
}
const QList<Attribute*>& AArray::GetAttributes() const
{
    return myValues;
}
void AArray::AddRow(int _index)
{
    if (!myTemplate.GetSharedParam().ignoreMax && myValues.count() >= myTemplate.GetSharedParam().max_i)
        return;

    if (_index < 0 || _index > myValues.count())
        _index = myValues.count();

    myValues.insert(_index, myTemplate.GetSharedParam().templateAtt->GenerateAttribute());
}
void AArray::DuplicateRow(int _index)
{
    if (_index < 0 || _index >= myValues.count())
        _index = myValues.count() - 1;

    myValues.insert(_index +1, myValues[_index]->CreateDuplica());
}
void AArray::RemoveRow(int _index)
{
    if (!myTemplate.GetSharedParam().ignoreMin && myValues.count() <= myTemplate.GetSharedParam().min_i)
        return;

    if (_index < 0 || _index >= myValues.count())
        return;

    Attribute* removed = myValues.takeAt(_index);
    removed->PreManualDelete();
    delete(removed);
}
void AArray::Empty()
{
    for (auto* elem : myValues)
    {
        elem->PreManualDelete();
        delete elem;
    }
    myValues.clear();
}
void AArray::MoveRow(int _originalIndex, int _targetIndex)
{
    const int valuesCount = myValues.count();
    if (_originalIndex < 0 || _originalIndex >= valuesCount)
        _originalIndex = valuesCount -1;
    if (_targetIndex < 0 || _targetIndex >= valuesCount)
        _targetIndex = valuesCount -1;

    if (_originalIndex == _targetIndex)
        return;

    auto* row = myValues.takeAt(_originalIndex);
    if (_originalIndex < _targetIndex)
        _targetIndex--;
    myValues.insert(_targetIndex, row);
}

QString AArray::GetShortDisplayedString(int _count)
{
    return QString("[ARRAY:%1]").arg(_count);
}
QString AArray::GetStructureStringFromList(const QStringList& _listString)
{
    QString _text = "[";
    for (int i = 0; i < _listString.count(); i++)
    {
        if (i > 0)
            _text.append(',');
        _text.append(_listString[i]);
    }
    _text.append(']');
    return _text;
}
