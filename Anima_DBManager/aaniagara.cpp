#include "aaniagara.h"

AANiagara::AANiagara() :
    AAsset()
{}

AANiagara::AANiagara(QString _filePath) :
    AAsset(_filePath)
{}


Attribute* AANiagara::CreateDuplica() const
{
    return new AANiagara(filePath);
}

void AANiagara::WriteValue_CSV(std::ofstream& file) const
{
    qFatal("Need to check the corret prefix");
    file << "NiagaraSystem";
    file << GetDisplayedText().toStdString();
}
