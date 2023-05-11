#ifndef STRUCTUREIMPORTHELPER_H
#define STRUCTUREIMPORTHELPER_H

#include <QString>
#include <QStringList>

namespace StructureImportHelper
{
    bool DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey = false);
};

#endif // STRUCTUREIMPORTHELPER_H
