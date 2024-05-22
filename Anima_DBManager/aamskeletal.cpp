#include "aamskeletal.h"

AAMSkeletal::AAMSkeletal(TemplateAttribute& _template) :
    AAMesh(_template)
{}

AAMSkeletal::AAMSkeletal(TemplateAttribute& _template, QString _filePath) :
    AAMesh(_template, _filePath)
{}

const QString& AAMSkeletal::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.SkeletalMesh";
    return className;
}
