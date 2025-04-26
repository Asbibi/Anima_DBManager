#ifndef STRUCTUREIMPORTHELPER_H
#define STRUCTUREIMPORTHELPER_H

#include <QString>
#include <QStringList>
#include <QList>
#include "attributetype.h"

namespace StructureImportHelper
{
    enum class OverwritePolicy
    {
        Overwrite,
        KeepExisting,
        NewRow
    };

    struct AttributeTemplateRepresentation
    {
        QString name;
        QString cppType;
        AttributeTypeHelper::Type dbType = AttributeTypeHelper::Type::Invalid;
        QString subRefStructOrEnumName;
        AttributeTypeHelper::Type subArryDbType = AttributeTypeHelper::Type::Invalid;
        QString subArrySubRefStructOrEnumName;
        QString defaultValue;
        bool active = true;
    };

    bool DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey = false);
    bool DecomposeCPPFile(const QString& _cppFileContent);

    QString RemoveCommentBlocks(const QString& _text);
    QString RemoveCommentLines(const QString& _text);
    QString GetStructName(const QString& _text);
    QString GetBracketContent(const QString& _text);

    void GetTemplateAttributeRepresentation(const QString& _structCppContent, const QList<QString>& _otherStructKeys, QList<AttributeTemplateRepresentation>& _outTemplRepresentations);
    void GetDbTypeFromCpp(const QList<QString>& _otherStructKeys, AttributeTemplateRepresentation& _outAttrRep);
    AttributeTypeHelper::Type GetTypeFromCppString(const QString& _typeCppString);
    bool ValidateEnumStructTypeAndGetSubType(const QList<QString>& _otherStructKeys, AttributeTypeHelper::Type _foundType, const QString& _cppType, QString& _subType);
}

#endif // STRUCTUREIMPORTHELPER_H
