#include "aamesh.h"

AAMesh::AAMesh(const AttributeParam& _sharedParam, bool _isSkeletal) :
    AAsset(_sharedParam),
    isSkeletal(_isSkeletal)
{}

AAMesh::AAMesh(const AttributeParam& _sharedParam, QString _filePath, bool _isSkeletal) :
    AAsset(_sharedParam, _filePath),
    isSkeletal(_isSkeletal)
{}


Attribute* AAMesh::CreateDuplica() const
{
    return new AAMesh(mySharedParam, filePath, isSkeletal);
}

void AAMesh::WriteValue_CSV(std::ofstream& file) const
{
    file << (isSkeletal ? "SkeletalMesh" : "StaticMesh");
    file << GetDisplayedText().toStdString();
}
