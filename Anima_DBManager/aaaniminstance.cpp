#include "aaaniminstance.h"

AAAnimInstance::AAAnimInstance(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AAAnimInstance::AAAnimInstance(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AAAnimInstance::CreateDuplica() const
{
    return new AAAnimInstance(mySharedParam, filePath);
}

void AAAnimInstance::WriteValue_CSV(std::ofstream& file) const
{
    file << "Class";
    AAsset::WriteValue_CSV(file);
}
