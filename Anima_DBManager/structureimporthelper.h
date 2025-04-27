#ifndef STRUCTUREIMPORTHELPER_H
#define STRUCTUREIMPORTHELPER_H

#include <QString>
#include <QStringList>
#include <QList>
#include "attributetype.h"
#include "templatestructure.h"

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
    QStringList DecomposeCPPFile(const QString& _cppFileContent);

    QString RemoveCommentBlocks(const QString& _text);
    QString RemoveCommentLines(const QString& _text);
    QString GetStructName(const QString& _text);
    QString GetBracketContent(const QString& _text);

    void GetTemplateAttributeRepresentation(const QString& _structCppContent, const QList<QString>& _otherStructKeys, QList<AttributeTemplateRepresentation>& _outTemplRepresentations);
    void GetDbTypeFromCpp(const QList<QString>& _otherStructKeys, AttributeTemplateRepresentation& _outAttrRep);
    AttributeTypeHelper::Type GetTypeFromCppString(const QString& _typeCppString);
    bool ValidateEnumStructStringTypeAndGetSubType(const QList<QString>& _otherStructKeys, AttributeTypeHelper::Type& _foundType, const QString& _cppType, const QString& _name, QString& _subType);

    bool GetStructureTemplateFromCppString(const QString& _structCppContent, const QMap<QString,QString>& _otherStructSections, TemplateStructure& _outTemplate);
    TemplateAttribute* NewAttributeFromRepresentation(const QMap<QString,QString>& _otherStructSections, const AttributeTemplateRepresentation& _representation);
    TemplateAttribute* NewAttributeFromRepresentation(const QMap<QString,QString>& _otherStructSections, const QString& _name, AttributeTypeHelper::Type _type, const QString& _subRef, AttributeTypeHelper::Type _subType, const QString& _subSubRef, bool _active, const QString _defVal);

}

#endif // STRUCTUREIMPORTHELPER_H
