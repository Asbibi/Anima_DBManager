#include "templatestructure.h"

#include <QDebug>

#include "sstringhelper.h"

TemplateStructure::TemplateStructure(const QString& _structName, const QColor& _structColor) :
    TemplateStructure(_structName, _structName.left(2).toUpper(), _structColor)
{}
TemplateStructure::TemplateStructure(const QString& _structName, const QString& _structAbbrev, const QColor& _structColor) :
    myStructColor(_structColor),
    myStructName(_structName),
    myStructAbbrev(_structAbbrev)
{}
TemplateStructure::TemplateStructure(const TemplateStructure& _other) :    
    TemplateStructure(_other.myStructName, _other.myStructAbbrev, _other.myStructColor)
{
    for (const auto& otherTemplAttr : _other.myAttributeTemplates)
        myAttributeTemplates.append(new TemplateAttribute(*otherTemplAttr));
}
void TemplateStructure::operator=(const TemplateStructure& _another)
{
    myStructColor = _another.myStructColor;
    myStructName = _another.myStructName;
    myStructAbbrev = _another.myStructAbbrev;

    myAttributeTemplates.clear();
    for (const auto& otherTemplAttr : _another.myAttributeTemplates)
        myAttributeTemplates.push_back(otherTemplAttr);
}
TemplateStructure::~TemplateStructure()
{
    while (!myAttributeTemplates.isEmpty())
    {
        delete myAttributeTemplates.takeFirst();
    }
}



void TemplateStructure::AddAttributeTemplate(int _index)
{
    AddAttributeTemplateInternal(new TemplateAttribute(), nullptr, _index);
}
void TemplateStructure::AddAttributeTemplate(const AttributeTypeHelper::Type _type, const QString& _attName, int _index)
{
    AddAttributeTemplateInternal(new TemplateAttribute(_attName, _type, AttributeParam()), nullptr, _index);
}
void TemplateStructure::AddAttributeTemplate(const AttributeTypeHelper::Type _type, const QString& att_Name, const AttributeParam& _attParam, int _index)
{
    AddAttributeTemplateInternal(new TemplateAttribute(att_Name, _type, _attParam), nullptr, _index);
}
void TemplateStructure::AddAttributeTemplate(const TemplateAttribute& _attTemplateToCopy, const QString* _newName, int _index)
{
    AddAttributeTemplateInternal(new TemplateAttribute(_attTemplateToCopy), _newName, _index);
}
void TemplateStructure::AddAttributeTemplateInternal(TemplateAttribute* _attTemplateToCopy, const QString* _newName, int _index)
{
    const int count = (int)(myAttributeTemplates.count());
    if (_index < 0 || _index > count)
        _index = count;

    myAttributeTemplates.insert(_index, _attTemplateToCopy);

    if (_newName)
        myAttributeTemplates[_index]->SetName(*_newName);
}
void TemplateStructure::RemoveAttribute(int _index)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    auto removedAttribute = myAttributeTemplates.takeAt(_index);
    delete removedAttribute;
}
void TemplateStructure::RemoveAttribute(const QString& _attName)
{
    RemoveAttribute(GetAttributeIndex(_attName));
}

void TemplateStructure::MoveAttribute(int _indexFrom, int _indexTo)
{
    auto templAttrib = myAttributeTemplates.takeAt(_indexFrom);
    myAttributeTemplates.insert(_indexTo, templAttrib);
}

void TemplateStructure::SetAttributeFromList(const QList<QString>& _stringList, QHash<AReference*, QString>& _outRefMap)
{
    // no use for this method outside of the Open action, if nec adapt it later to be usable at any state
    Q_ASSERT(myAttributeTemplates.count() == 0);

    int attrCount = _stringList.count();
    for (int i = 0; i < attrCount; i++)
    {
        const QString& stringAttr = _stringList[i];

        AttributeTypeHelper::Type type = AttributeTypeHelper::StringToType(stringAttr.section('|', 1, 1));
        AttributeParam param = AttributeParam(stringAttr.section('|', 2, -2), _outRefMap);
        AddAttributeTemplate(type, stringAttr.section('|', 0, 0), param, i);

        // Reference attribute initalisation is deleguated to the map owner
        if (type == AttributeTypeHelper::Type::Reference)
        {
            AReference* aref = dynamic_cast<AReference*>(myAttributeTemplates[i]->GetDefaultAttributeW());
            Q_ASSERT(aref != nullptr);
            _outRefMap.insert(aref, stringAttr.section('|', -1, -1));
        }
        else
        {
            myAttributeTemplates[i]->GetDefaultAttributeW()->ReadValue_CSV(stringAttr.section('|', -1, -1));
        }
    }
}
bool TemplateStructure::ChangeAttribute(int _attrIndex, const TemplateAttribute& _templateToCopy)
{
    return GetAttributeTemplate(_attrIndex)->SetNewValues(_templateToCopy);
}


void TemplateStructure::RenameAttributeTemplate(int _index, QString& _newName)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;
    if (myAttributeTemplates[_index]->GetName() == _newName)
        return;

    QString baseName = _newName;
    auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetAttributeIndex(_identifier) == -1); };
    _newName = SStringHelper::GetUniqueIdentifier(baseName, validate, true);

    myAttributeTemplates[_index]->SetName(_newName);
}
void TemplateStructure::SetAttributeDefaultValue(int _index, const QString& _value)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    myAttributeTemplates[_index]->SetDefaultValue(_value);
}
void TemplateStructure::SetAttributeDefaultValue(const QString& _attName, const QString& _value)
{
    SetAttributeDefaultValue(GetAttributeIndex(_attName), _value);
}
int TemplateStructure::GetAttributeIndex(const QString& _attName) const
{
    int index = 0;
    for (const auto& attr : myAttributeTemplates)
    {
        if (attr->GetName() == _attName)
            return index;
        index++;
    }
    return -1;
}


AttributeTypeHelper::Type TemplateStructure::GetAttributeType(int _index) const
{
    const TemplateAttribute* attr = GetAttributeTemplate(_index);
    return attr ? attr->GetType() : AttributeTypeHelper::Type::Invalid;
}
const QString& TemplateStructure::GetAttributeName(int _index) const
{
    return myAttributeTemplates[_index]->GetName();
}
TemplateAttribute* TemplateStructure::GetAttributeTemplate(int _index)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return nullptr;

    return myAttributeTemplates[_index];
}
const TemplateAttribute* TemplateStructure::GetAttributeTemplate(int _index) const
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return nullptr;

    return myAttributeTemplates[_index];
}
const TemplateAttribute* TemplateStructure::GetAttributeTemplate(const QString& att_Name) const
{
    return GetAttributeTemplate(GetAttributeIndex(att_Name));
}


void TemplateStructure::SaveTemplate_CSV(std::ofstream& file) const
{
    file << "###" << myStructName.toStdString() << "---"
    << myStructAbbrev.toStdString()  << "---"
    << myStructColor.name().toStdString() << "###\n";

    for (const auto& templateAttr : myAttributeTemplates)
    {
        templateAttr->SaveTemplate_CSV(file);
        file << "\n";
    }
}
