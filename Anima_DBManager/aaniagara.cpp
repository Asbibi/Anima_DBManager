#include "aaniagara.h"

AANiagara::AANiagara(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AANiagara::AANiagara(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AANiagara::CreateDuplica() const
{
    return new AANiagara(mySharedParam, filePath);
}

void AANiagara::WriteValue_CSV(std::ofstream& file) const
{
    qFatal("Need to check the corret prefix");
    file << "NiagaraSystem";
    file << GetDisplayedText().toStdString();
}
