#include "attributeparam.h"
#include "attribute.h"


AttributeParam::~AttributeParam()
{
    if (templateAtt != nullptr)
        delete(templateAtt);
}
