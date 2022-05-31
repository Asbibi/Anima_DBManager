#include "aasound.h"

AASound::AASound() :
    AAsset()
{}

AASound::AASound(QString _filePath) :
    AAsset(_filePath)
{}


Attribute* AASound::CreateDuplica() const
{
    return new AASound(filePath);
}

void AASound::WriteValue_CSV(std::ofstream& file) const
{
    qFatal("Need to set the corret prefix");
    file << "??? - Sound - ???";
    file << GetDisplayedText().toStdString();
}
