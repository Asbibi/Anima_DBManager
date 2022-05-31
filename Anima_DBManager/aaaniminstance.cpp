#include "aaaniminstance.h"

AAAnimInstance::AAAnimInstance()
{}

AAAnimInstance::AAAnimInstance(QString _filePath) :
    AAsset(_filePath)
{}


Attribute* AAAnimInstance::CreateDuplica() const
{
    return new AAAnimInstance(filePath);
}

void AAAnimInstance::WriteValue_CSV(std::ofstream& file) const
{
    file << "Class";
    file << GetDisplayedText().toStdString();
}
