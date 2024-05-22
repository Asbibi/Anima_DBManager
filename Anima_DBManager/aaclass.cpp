#include "aaclass.h"

AAClass::AAClass(TemplateAttribute& _template) :
    AAsset(_template)
{}

AAClass::AAClass(TemplateAttribute& _template, QString _filePath) :
    AAsset(_template, _filePath)
{}


const QString& AAClass::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.BlueprintGeneratedClass";
    return className;
}
