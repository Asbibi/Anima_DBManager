#include "aatexture.h"

AATexture::AATexture(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AATexture::AATexture(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AATexture::CreateDuplica() const
{
    return new AATexture(mySharedParam, filePath);
}

void AATexture::WriteValue_CSV(std::ofstream& file) const
{
    file << "Texture2D";
    file << GetDisplayedText().toStdString();
}
