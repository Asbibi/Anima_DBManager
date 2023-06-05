#include "attributeparam.h"

#include "templateattribute.h"
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
    structTable {_another.structTable },
    enumeratorIndex {_another.enumeratorIndex }
{
    if (_another.templateAtt != nullptr)
    {
        templateAtt = new TemplateAttribute(*_another.templateAtt);
    }
    DB_Manager::GetDB_Manager().RegisterAttributeParam(this);
}
AttributeParam::AttributeParam(const QString& _csvString, QHash<AReference*, QString>& _outRefMap) :
    ignoreMin{ _csvString.section(',',0,0) == "TRUE" },
    ignoreMax { _csvString.section(',',1,1) == "TRUE" },
    min_i { _csvString.section(',',2,2).toInt() },
    max_i { _csvString.section(',',3,3).toInt() },
    min_f { _csvString.section(',',4,4).toFloat() },
    max_f { _csvString.section(',',5,5).toFloat() },
    templateAtt { TemplateAttribute::NewAttribute_CSV(CleanTemplateStringCSV(_csvString.section(',',6,6)), _outRefMap) },
    structTable { DB_Manager::GetDB_Manager().GetStructureTable(_csvString.section(',',7,7).toInt()) },
    enumeratorIndex { _csvString.section(',',8,8).toInt() }
{}
AttributeParam::~AttributeParam()
{
    if (templateAtt != nullptr)
        delete(templateAtt);

    DB_Manager::GetDB_Manager().UnregisterAttributeParam(this);
}
void AttributeParam::operator=(const AttributeParam& _another)
{
    ignoreMin = _another.ignoreMin;
    ignoreMax = _another.ignoreMax;
    min_i = _another.min_i;
    max_i = _another.max_i;
    min_f = _another.min_f;
    max_f = _another.max_f;
    structTable = _another.structTable;
    enumeratorIndex = _another.enumeratorIndex;

    if (templateAtt != nullptr)
    {
        delete(templateAtt);
        templateAtt = nullptr;
    }

    if (_another.templateAtt)
        templateAtt = new TemplateAttribute(*_another.templateAtt);
    else
        templateAtt = nullptr;
}


const Enumerator* AttributeParam::GetEnum() const
{
    return DB_Manager::GetDB_Manager().GetEnum(enumeratorIndex);
}

void AttributeParam::SaveParams_CSV(std::ofstream& file) const
{
    file << GetParamsAsCSV().toStdString();
}
QString AttributeParam::GetParamsAsCSV() const
{
    QString paramAsCSV = QString(ignoreMin ? "TRUE" : "FALSE") + ','
            + (ignoreMax ? "TRUE" : "FALSE") + ','
            + QString::number(min_i) + ','
            + QString::number(max_i) + ','
            + QString::number(min_f) + ','
            + QString::number(max_f) + ',';
    if (templateAtt != nullptr)
    {
        paramAsCSV += '[' + templateAtt->GetTemplateAsCSV().replace(',', ';') + ']';
    }

    return paramAsCSV + ','
            + QString::number(structTable != nullptr ? DB_Manager::GetDB_Manager().GetStructureTableIndex(structTable->GetTemplateName()) : -1 ) + ','
            + QString::number(enumeratorIndex);
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
