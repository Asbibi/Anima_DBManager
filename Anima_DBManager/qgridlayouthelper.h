#ifndef QGRIDLAYOUTHELPER_H
#define QGRIDLAYOUTHELPER_H

struct GridIndexes
{
    int row;
    int col;
};

namespace QGridLayoutHelper
{

GridIndexes GetGridIndexes(int _elementIndex);

}

#endif // QGRIDLAYOUTHELPER_H
