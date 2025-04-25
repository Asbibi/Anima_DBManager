#ifndef STRUCTUREIMPORTHELPER_H
#define STRUCTUREIMPORTHELPER_H

#include <QString>
#include <QStringList>

namespace StructureImportHelper
{
    enum class OverwritePolicy
    {
        Overwrite,
        KeepExisting,
        NewRow
    };

    bool DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey = false);
    bool DecomposeCPPFile(const QString& _cppFileContent);

    QString RemoveCommentBlocks(const QString& _text);
    QString GetStructName(const QString& _text);
    QString GetBracketContent(const QString& _text);
}

#endif // STRUCTUREIMPORTHELPER_H
