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
        AttributeTypeHelper::Type dbType;
        QString defaultValue;
        bool active = true;
    };

    bool DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey = false);
    bool DecomposeCPPFile(const QString& _cppFileContent);

    QString RemoveCommentBlocks(const QString& _text);
    QString RemoveCommentLines(const QString& _text);
    QString GetStructName(const QString& _text);
    QString GetBracketContent(const QString& _text);

    void GetTemplateAttributeRepresentation(const QString& _structCppContent, QList<AttributeTemplateRepresentation>& _templRepresentations);
}

#endif // STRUCTUREIMPORTHELPER_H
