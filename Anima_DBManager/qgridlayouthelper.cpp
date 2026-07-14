#include "qgridlayouthelper.h"

#define MAX_COLUMN 4


GridIndexes QGridLayoutHelper::GetGridIndexes(int _elementIndex)
{
    GridIndexes indexes {};
    indexes.row = _elementIndex / MAX_COLUMN;
    indexes.col = _elementIndex % MAX_COLUMN;
    return indexes;
}


#undef MAX_COLUMN