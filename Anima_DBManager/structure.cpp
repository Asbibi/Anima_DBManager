#include "structure.h"

#include "db_manager.h"
#include "aarray.h"
#include <QList>


Structure::Structure(TemplateStructure& _structureTemplate) :
    myTemplate(_structureTemplate)
{
    const QList<TemplateAttribute*>& _templateAttributes = _structureTemplate.GetAttributes();
    for (const auto* _templ : _templateAttributes)
        myAttributes.push_back(_templ->GenerateAttribute());
}
Structure::Structure(const Structure& _other, bool _attrDeepCopy) :
    QObject(),
    myTemplate(_other.myTemplate)
{
    if (_attrDeepCopy)
    {
        for (auto* _att : _other.myAttributes)
            myAttributes.push_back(_att->CreateDuplica());
    }
    else
    {
        for (auto* _att : _other.myAttributes)
            myAttributes.push_back(_att);
    }
}
Structure::Structure(const Structure& _other, TemplateStructure& _newTemplate) :
    myTemplate(_newTemplate)
{
    const int myAttrCount = myTemplate.GetAttributesCount();
    for (int i = 0; i < myAttrCount; i++)
    {
        auto* newAttr = myTemplate.GetAttributeTemplate(i)->GenerateAttribute();
        newAttr->CopyValueFromOther(_other.GetAttribute(i));
        myAttributes.push_back(newAttr);
    }
}
Structure::~Structure()
{
    for(auto const& att : myAttributes)
    {
        att->PreManualDelete();
        delete(att);
    }
}




const TemplateStructure& Structure::GetTemplate() const
{
    return myTemplate;
}
int Structure::GetAttributeCount() const
{
    return (int)myAttributes.size();
}
const Attribute* Structure::GetAttribute(int _attIndex) const
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return nullptr;

    return myAttributes[_attIndex];
}
const Attribute* Structure::GetAttribute(const QString& _attName) const
{
    return GetAttribute(myTemplate.GetAttributeIndex(_attName));
}
Attribute* Structure::GetAttribute(int _attIndex)
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return nullptr;

    return myAttributes[_attIndex];
}
const QList<Attribute*>& Structure::GetAttributes() const
{
    return myAttributes;
}
void Structure::SetAttributeValueFromText(int _attIndex, const QJsonValue& _value)
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return;
    myAttributes[_attIndex]->SetValue_JSON(_value);
}
void Structure::SetAttributeValueFromText(const QString& _attName, const QJsonValue& _value)
{
    SetAttributeValueFromText(myTemplate.GetAttributeIndex(_attName), _value);
}
void Structure::ReadValue_JSON(const QJsonObject& _structAsJson)
{
    const auto& dbManager = DB_Manager::GetDB_Manager();
    for (int i = 0; i < myAttributes.size(); i++)
    {
        const QString& attrName = myTemplate.GetAttributeName(i);
        const bool jsonReadOk = myAttributes[i]->SetValue_JSON(_structAsJson.value(dbManager.GetAttributeFullName(attrName)));
        if (!jsonReadOk)
        {
            qWarning() << _structAsJson.value("Name").toString() << " - Ignored " << attrName << " Attribute value : invalid json type" << _structAsJson.value(attrName);
        }
    }
}
void Structure::ReadAttributeValue_CSV(int _attIndex, const QString& _csvValue)
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return;
    myAttributes[_attIndex]->SetValue_CSV(_csvValue);
}
void Structure::MoveAttribute(int _indexFrom, int _indexTo)
{
    auto* attr = myAttributes.takeAt(_indexFrom);
    myAttributes.insert(_indexTo, attr);
}
void Structure::ResetAttributeToDefault(int _attindex)
{
    myAttributes[_attindex]->CopyValueFromOther(myTemplate.GetAttributeTemplate(_attindex)->GetDefaultAttribute());
}
void Structure::FixAttributeTypeToDefault(int _attIndex)
{
    // Replace the attribute with another that has to correct class considering its type
    auto* attr = myAttributes[_attIndex];
    myAttributes[_attIndex] = myTemplate.GetAttributeTemplate(_attIndex)->GenerateAttribute();

    // Delete the old invalid one
    attr->PreManualDelete();
    if (attr->GetType() == AttributeTypeHelper::Type::Array)
    {
        AArray* aa = dynamic_cast<AArray*>(attr);
        aa->Empty(false);
    }
    delete attr;
}
void Structure::AddAttribute(int _position, bool _copyFromPrevious)
{
    myAttributes.insert(_position, myTemplate.GetAttributeTemplate(_position)->GenerateAttribute());
    if (_copyFromPrevious)
        myAttributes[_position]->CopyValueFromOther(myAttributes[_position -1]);
}
void Structure::RemoveAttribute(int _position)
{
    auto* removedAttr = myAttributes.takeAt(_position);
    removedAttr->PreManualDelete();
    delete removedAttr;
}



QJsonObject Structure::WriteValue_JSON_AsRow() const
{
    QJsonObject structAsJSON = QJsonObject();

    const auto& dbManager = DB_Manager::GetDB_Manager();
    for (int i = 0; i < myAttributes.size(); i++)
    {
        structAsJSON.insert(dbManager.GetAttributeFullName(myTemplate.GetAttributeName(i)), myAttributes[i]->GetValue_JSON());
    }

    return structAsJSON;
}
void Structure::WriteValue_CSV_AsRow(std::ofstream& file) const
{
    // Add all attribute values in between ""
    for (const auto& att : myAttributes)
    {
        file << ",\"";
        att->WriteValue_CSV(file);
        file << "\"";
    }
    file << "\n";
}
QString Structure::GetStructureAsCSV() const
{
    QString structAsCSV = "(";

    // === Add all attributes ===
    for (int i = 0; i < myAttributes.size(); i++)
    {
        if (0 < i)
        {
            structAsCSV.append(",");
        }
        structAsCSV.append(DB_Manager::GetDB_Manager().GetAttributeFullName(myTemplate.GetAttributeName(i)));
        structAsCSV.append('=');
        const auto type = myAttributes[i]->GetType();
        if (AttributeTypeHelper::ShouldBeWrappedInQuoteInCSV(type))
        {
            structAsCSV.append(QString(AttributeTypeHelper::csvDoubleQuoteWrapper).arg(myAttributes[i]->GetValue_CSV()));
        }
        else
        {
            structAsCSV.append(myAttributes[i]->GetValue_CSV());
        }
    }
    // ==========================

    structAsCSV.append(')');
    return structAsCSV;
}
QString Structure::GetDisplayText() const
{
    QString text = "";
    const auto& attributeTemplates = myTemplate.GetAttributes();
    const int nAttr = attributeTemplates.count();
    for (int i = 0; i < nAttr; i++)
    {
        text += attributeTemplates[i]->GetName() + " \t = " + GetAttribute(i)->GetValue_String() + "\n";
    }
    return text;
}

bool Structure::IsOneOfMyAttributes(const Attribute* _attributePtr) const
{
    return myAttributes.indexOf(_attributePtr) != -1;
}
