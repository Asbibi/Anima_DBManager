#include "aatexture.h"

AATexture::AATexture() :
    AAsset()
{}

AATexture::AATexture(QString _filePath) :
    AAsset(_filePath)
{}


Attribute* AATexture::CreateDuplica() const
{
    return new AATexture(filePath);
}

void AATexture::WriteValue_CSV(std::ofstream& file) const
{
    file << "Texture2D";
    file << GetDisplayedText().toStdString();
}
