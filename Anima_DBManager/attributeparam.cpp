#include "attributeparam.h"

#include "attribute.h"
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
    templateAtt {_another.templateAtt },
    structTable {_another.structTable },
    enumeratorIndex {_another.enumeratorIndex }
{
    DB_Manager::GetDB_Manager().RegisterAttributeParam(this);
}
AttributeParam::~AttributeParam()
{
    if (templateAtt != nullptr)
        delete(templateAtt);

    DB_Manager::GetDB_Manager().UnregisterAttributeParam(this);
}


const Enumerator* AttributeParam::GetEnum() const
{
    return DB_Manager::GetDB_Manager().GetEnum(enumeratorIndex);
}
