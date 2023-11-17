#include "templatestructure.h"

#include "sstringhelper.h"
#include "savemanager.h"

#include <QDebug>


TemplateStructure::TemplateStructure() :
    TemplateStructure("", QColorConstants::Black)
{}
TemplateStructure::TemplateStructure(const QString& _structName, const QColor& _structColor, IconManager::IconType _iconType) :
    TemplateStructure(_structName, _structName.left(2).toUpper(), _structColor, _iconType)
{}
TemplateStructure::TemplateStructure(const QString& _structName, const QString& _structAbbrev, const QColor& _structColor, IconManager::IconType _iconType) :
    myStructColor(_structColor),
    myIconType (_iconType),
    myStructName(_structName),
    myStructAbbrev(_structAbbrev)
{}
TemplateStructure::TemplateStructure(const TemplateStructure& _other) :    
    TemplateStructure(_other.myStructName, _other.myStructAbbrev, _other.myStructColor, _other.myIconType)
{
    for (const auto& otherTemplAttr : _other.myAttributeTemplates)
        myAttributeTemplates.append(new TemplateAttribute(*otherTemplAttr));
}
TemplateStructure::~TemplateStructure()
{
    while (!myAttributeTemplates.isEmpty())
    {
        delete myAttributeTemplates.takeFirst();
    }
}


void TemplateStructure::SetNewValues(const TemplateStructure& _templateToCopy)
{
    const int newTemplateCount = _templateToCopy.GetAttributesCount();
    while (GetAttributesCount() > newTemplateCount)
    {
        RemoveAttribute(newTemplateCount);
    }

    const int currAttributeCount = GetAttributesCount();
    for (int i = 0; i < currAttributeCount; i++)
    {
        myAttributeTemplates[i]->SetNewValues(*_templateToCopy.myAttributeTemplates[i]);
    }
    for (int i = currAttributeCount; i < newTemplateCount; i++)
    {
        AddAttributeTemplate(*_templateToCopy.myAttributeTemplates[i], nullptr, i);
    }
}

void TemplateStructure::ReabbrevStructureTemplate(QString& _newStructAbbrev)
{
    SStringHelper::CleanStringForIdentifier(_newStructAbbrev);
    _newStructAbbrev = _newStructAbbrev.toUpper();
    myStructAbbrev = _newStructAbbrev;
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
    AddAttributeTemplateInternal(new TemplateAttribute("", _type, _attParam), &att_Name, _index);
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
    QString newAttributeName = _newName != nullptr ? *_newName : _attTemplateToCopy->GetName();
    RenameAttributeTemplate(_index, newAttributeName, true);
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

bool TemplateStructure::ChangeAttribute(int _attrIndex, const TemplateAttribute& _templateToCopy)
{
    return GetAttributeTemplate(_attrIndex)->SetNewValues(_templateToCopy);
}


void TemplateStructure::RenameAttributeTemplate(int _index, QString& _newName, bool _skipSameNameCheck)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    SStringHelper::CleanStringForIdentifier(_newName);
    if (!_skipSameNameCheck && myAttributeTemplates[_index]->GetName() == _newName)
        return;

    QString baseName = _newName;
    auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetAttributeIndex(_identifier) == -1); };
    _newName = SStringHelper::GetUniqueIdentifier(baseName, validate, true);

    if (DB_Manager::GetDB_Manager().GetAttributeFullName(_newName) == "Name")
    {
        qDebug() << "Tried to set \"Name\" as an attribute name which is not allowed due to JSON exporting";
        _newName = myAttributeTemplates[_index]->GetName();
        return;
    }

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


void TemplateStructure::SaveTemplate(QJsonArray& _templateJson) const
{
    QJsonObject thisAsJson = QJsonObject();

    thisAsJson.insert("Name", myStructName);
    thisAsJson.insert("Abbrev", myStructAbbrev);
    thisAsJson.insert("Icon", (int)myIconType);
    thisAsJson.insert("Color", myStructColor.name());

    QJsonArray myAttributesAsJson = QJsonArray();
    for (const auto& templateAttr : myAttributeTemplates)
    {
        myAttributesAsJson.push_back(templateAttr->GetAsJson());
    }
    thisAsJson.insert("Attributes", myAttributesAsJson);

    _templateJson.push_back(thisAsJson);
}
TemplateStructure TemplateStructure::LoadTemplateNoAttribute(const QJsonObject& _templateJson)
{
    auto newTemplate = TemplateStructure(_templateJson.value("Name").toString(),
                                         _templateJson.value("Abbrev").toString(),
                                         QColor(_templateJson.value("Color").toString()),
                                         IconManager::IconType(_templateJson.value("Icon").toInt()));

    return newTemplate;
}
void TemplateStructure::LoadTemplateOnlyAttribute(const QJsonArray& _templateJson)
{
    int i = 0;
    Q_ASSERT(myAttributeTemplates.count() == 0);
    myAttributeTemplates.reserve(_templateJson.count());
    for (const auto& attrTemplateJson : _templateJson)
    {
        TemplateAttribute* temporaryAttr = TemplateAttribute::NewAttributeFromJSON(attrTemplateJson.toObject());
        myAttributeTemplates.insert(i, temporaryAttr);
        i++;
    }
}
