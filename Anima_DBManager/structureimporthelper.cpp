#include "structureimporthelper.h"


bool StructureImportHelper::DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey)
{
    int firstComma = _csvLineCopy.indexOf(',');
    if (firstComma == -1)
    {
        return false;
    }

    _csvLineCopy.remove(_csvLineCopy.length()-1,1);
    _csvLineCopy.replace(firstComma, 1, "\",");
    _outList = _csvLineCopy.split("\",\"");
    if (_outList.count() != _expectedCountWithKey)
    {
        return false;
    }

    if (_removeKey)
    {
        _outList.removeFirst();
    }

    return true;
}
