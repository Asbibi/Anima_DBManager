#include "aamesh.h"

AAMesh::AAMesh(bool _isSkeletal) :
    AAsset(),
    isSkeletal(_isSkeletal)
{}

AAMesh::AAMesh(QString _filePath, bool _isSkeletal) :
    AAsset(_filePath),
    isSkeletal(_isSkeletal)
{}


Attribute* AAMesh::CreateDuplica() const
{
    return new AAMesh(filePath, isSkeletal);
}

void AAMesh::WriteValue_CSV(std::ofstream& file) const
{
    file << (isSkeletal ? "SkeletalMesh" : "StaticMesh");
    file << GetDisplayedText().toStdString();
}
