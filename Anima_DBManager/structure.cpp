#include "structure.h"

Structure::Structure(const StructureTemplate& structureTemplate)
{
    const std::vector<const Attribute*>& templates = structureTemplate.GetAttributes();
    for (const auto& templ : templates)
        attributes.push_back(templ->CreateDuplica());
}
Structure::~Structure()
{
    for(auto const& att : attributes)
        delete(att);
}


