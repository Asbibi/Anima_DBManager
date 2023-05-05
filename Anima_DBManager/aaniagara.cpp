#include "aaniagara.h"

AANiagara::AANiagara(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AANiagara::AANiagara(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AANiagara::CreateDuplica() const
{
    return new AANiagara(mySharedParam, myFilePath);
}

const QString& AANiagara::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.NiagaraSystem";
    return className;
}
