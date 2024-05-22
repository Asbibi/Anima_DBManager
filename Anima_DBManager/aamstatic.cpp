#include "aamstatic.h"

AAMStatic::AAMStatic(TemplateAttribute& _template) :
    AAMesh(_template)
{}

AAMStatic::AAMStatic(TemplateAttribute& _template, QString _filePath) :
    AAMesh(_template, _filePath)
{}

const QString& AAMStatic::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.StaticMesh";
    return className;
}
