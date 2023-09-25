#include "aaniagara.h"

AANiagara::AANiagara(TemplateAttribute& _template) :
    AAsset(_template)
{}

AANiagara::AANiagara(TemplateAttribute& _template, QString _filePath) :
    AAsset(_template, _filePath)
{}


const QString& AANiagara::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.NiagaraSystem";
    return className;
}
