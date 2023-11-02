#include "attributeparam.h"

#include "templateattribute.h"
#include "templatestructure.h"
#include "db_manager.h"
#include "enumerator.h"
#include "areference.h"


AttributeParam::AttributeParam()
{
    DB_Manager::GetDB_Manager().RegisterAttributeParam(this);
}
AttributeParam::AttributeParam(const AttributeParam& _another) :
    ignoreMin{_another.ignoreMin},
    ignoreMax {_another.ignoreMax },
    min_i {_another.min_i },
    max_i {_another.max_i },
    min_f {_another.min_f },
    max_f {_another.max_f },
    templateAtt { nullptr },
    templateStruct{ nullptr },
    structTable {_another.structTable },
    enumeratorIndex {_another.enumeratorIndex }
{
    if (_another.templateAtt != nullptr)
    {
        templateAtt = new TemplateAttribute(*_another.templateAtt);
    }
    if (_another.templateStruct != nullptr)
    {
        templateStruct = new TemplateStructure(*_another.templateStruct);
    }
    DB_Manager::GetDB_Manager().RegisterAttributeParam(this);
}
AttributeParam::AttributeParam(const QJsonObject& _paramAsJson) :
    ignoreMin { _paramAsJson.value("ignoreMin").toBool()},
    ignoreMax { _paramAsJson.value("ignoreMax").toBool()},
    min_i { _paramAsJson.value("min_i").toInt()},
    max_i { _paramAsJson.value("max_i").toInt()},
    min_f { (float)_paramAsJson.value("min_f").toDouble()},
    max_f { (float)_paramAsJson.value("max_f").toDouble()},
    templateAtt { _paramAsJson.contains("templateAtt") ?
                    TemplateAttribute::NewAttributeFromJSON(_paramAsJson.value("templateAtt").toObject()) :
                    nullptr},
    templateStruct {nullptr},
    structTable { _paramAsJson.contains("structTable") ?
                      DB_Manager::GetDB_Manager().GetStructureTable(_paramAsJson.value("structTable").toString()) :
                      nullptr},
    enumeratorIndex { _paramAsJson.value("enumeratorIndex").toInt()}
{
    if (!_paramAsJson.contains("templateStruct"))
    {
         return;
    }
    templateStruct = new TemplateStructure();
    templateStruct->LoadTemplateOnlyAttribute(_paramAsJson.value("templateStruct").toArray());
}
AttributeParam::~AttributeParam()
{
    if (templateAtt != nullptr)
        delete(templateAtt);

    if (templateStruct != nullptr)
        delete(templateStruct);

    DB_Manager::GetDB_Manager().UnregisterAttributeParam(this);
}
void AttributeParam::operator=(const AttributeParam& _another)
{    
    // Simple fields Copy
    ignoreMin = _another.ignoreMin;
    ignoreMax = _another.ignoreMax;
    min_i = _another.min_i;
    max_i = _another.max_i;
    min_f = _another.min_f;
    max_f = _another.max_f;
    structTable = _another.structTable;
    enumeratorIndex = _another.enumeratorIndex;


    // templateAtt Copy
    if (_another.templateAtt)
    {
        if (templateAtt != nullptr)
        {
            templateAtt->SetNewValues(*_another.templateAtt);
        }
        else
        {
            templateAtt = new TemplateAttribute(*_another.templateAtt);
        }
    }
    else
    {
        if (templateAtt != nullptr)
        {
            delete(templateAtt);
            templateAtt = nullptr;
        }
    }


    // templateStruct Copy
    if (_another.templateStruct)
    {
        if (templateStruct != nullptr)
        {
            templateStruct->SetNewValues(*_another.templateStruct);
        }
        else
        {
            templateStruct = new TemplateStructure(*_another.templateStruct);
        }
    }
    else
    {
        if (templateStruct != nullptr)
        {
            delete(templateStruct);
            templateStruct = nullptr;
        }
    }
}


const Enumerator* AttributeParam::GetEnum() const
{
    return DB_Manager::GetDB_Manager().GetEnum(enumeratorIndex);
}

QJsonObject AttributeParam::GetAsJson() const
{
    QJsonObject param;
#define JSONIZE_DIRECT(field) param.insert(#field, field)

    JSONIZE_DIRECT(ignoreMin);
    JSONIZE_DIRECT(ignoreMax);
    JSONIZE_DIRECT(min_i);
    JSONIZE_DIRECT(max_i);
    JSONIZE_DIRECT(min_f);
    JSONIZE_DIRECT(max_f);
    JSONIZE_DIRECT(enumeratorIndex);

#undef JSONIZE_DIRECT

    if (templateAtt != nullptr)
    {
        param.insert("templateAtt", templateAtt->GetAsJson());
    }
    if (templateStruct != nullptr)
    {
        QJsonArray subStructJson = QJsonArray();
        for (const auto& templateAttr : templateStruct->GetAttributes())
        {
            subStructJson.push_back(templateAttr->GetAsJson());
        }
        param.insert("templateStruct", subStructJson);
    }
    if (structTable != nullptr)
    {
        param.insert("structTable", structTable->GetTemplateName());
    }

    return param;
}


QString AttributeParam::CleanTemplateStringCSV(const QString& _csv)
{
    if (_csv.isEmpty())
    {
        return "";
    }

    Q_ASSERT(_csv.length() > 1 && _csv[0] == '[');
    QString newCsv = _csv.sliced(1, _csv.length() - 2);
    newCsv.replace(';',',');

    AttributeTypeHelper::Type type = AttributeTypeHelper::StringToType(newCsv.section('|',1,1));
    if (type != AttributeTypeHelper::Type::Array && type != AttributeTypeHelper::Type::Structure)
        return newCsv;

    bool isArray = type == AttributeTypeHelper::Type::Array;
    int first = newCsv.indexOf(isArray ? '[' : '{');
    int last = newCsv.lastIndexOf(isArray ? ']' : '}');
    int count = newCsv.length();

    return newCsv.first(first) + newCsv.mid(first, last - first).replace(',',';') + newCsv.last(count - last);
}
