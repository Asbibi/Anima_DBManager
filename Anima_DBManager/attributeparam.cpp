#include "attributeparam.h"

#include "templateattribute.h"
#include "db_manager.h"
#include "enumerator.h"

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
AttributeParam::AttributeParam(const QString& _csvString) :
    ignoreMin{ _csvString.section(',',0,0) == "TRUE" },
    ignoreMax { _csvString.section(',',1,1) == "TRUE" },
    min_i { _csvString.section(',',2,2).toInt() },
    max_i { _csvString.section(',',3,3).toInt() },
    min_f { _csvString.section(',',4,4).toFloat() },
    max_f { _csvString.section(',',5,5).toFloat() },
    templateAtt { nullptr },                            // TODO use _csvString.section(',',6,6)
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
        templateAtt = _another.templateAtt->CreateDuplica();
    else
        templateAtt = nullptr;
}


const Enumerator* AttributeParam::GetEnum() const
{
    return DB_Manager::GetDB_Manager().GetEnum(enumeratorIndex);
}

void AttributeParam::SaveParams_CSV(std::ofstream& file) const
{
    file << (ignoreMin ? "TRUE" : "FALSE") << ',';
    file << (ignoreMax ? "TRUE" : "FALSE") << ',';
    file << min_i << ',';
    file << max_i << ',';
    file << min_f << ',';
    file << max_f << ',';
    file << -1 << ',';       // TODO
    file << (structTable != nullptr ? DB_Manager::GetDB_Manager().GetStructureTableIndex(structTable->GetTemplateName()) : -1 )<< ',';
    file << enumeratorIndex;
}
