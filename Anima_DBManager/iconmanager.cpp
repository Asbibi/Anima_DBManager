#include "iconmanager.h"

#include <QBitmap>
#include <QPixmap>
#include <Qt>

#define APP_ICON_NAME "DB_Icon.png"

IconManager::IconManager()
{
    myIconFileCharMap.insert(IconType::Square,              'A');
    myIconFileCharMap.insert(IconType::SquareHollow,        'B');
    myIconFileCharMap.insert(IconType::Circle,              'C');
    myIconFileCharMap.insert(IconType::CircleHollow,        'D');
    myIconFileCharMap.insert(IconType::TriangleUp,          'E');
    myIconFileCharMap.insert(IconType::TriangleUpHollow,    'F');
    myIconFileCharMap.insert(IconType::TriangleDown,        'G');
    myIconFileCharMap.insert(IconType::TriangleDownHollow,  'H');
    myIconFileCharMap.insert(IconType::Star,                'I');
    myIconFileCharMap.insert(IconType::StarHollow,          'J');
    myIconFileCharMap.insert(IconType::V,                   'K');
    myIconFileCharMap.insert(IconType::X,                   'L');
    myIconFileCharMap.insert(IconType::Anima,               'M');
    myIconFileCharMap.insert(IconType::AnimaHollow,         'N');
    myIconFileCharMap.insert(IconType::Stuff,               'O');
    myIconFileCharMap.insert(IconType::StuffHollow,         'P');
    myIconFileCharMap.insert(IconType::Map,                 'Q');
    myIconFileCharMap.insert(IconType::MapHollow,           'R');
    myIconFileCharMap.insert(IconType::Scroll,              'S');
    myIconFileCharMap.insert(IconType::ScrollHollow,        'T');
    myIconFileCharMap.insert(IconType::Face,                'U');
    myIconFileCharMap.insert(IconType::FaceHollow,          'V');
    myIconFileCharMap.insert(IconType::Text1,               'W');
    myIconFileCharMap.insert(IconType::Text2,               'X');
    myIconFileCharMap.insert(IconType::Text3,               'Y');
    myIconFileCharMap.insert(IconType::Text4,               'Z');
}
IconManager& IconManager::GetInstance()
{
    static IconManager im = IconManager();
    return im;
}


QIcon IconManager::GetAppIcon()
{
    return QIcon(GetInstance().myIconFolder + APP_ICON_NAME);
}
QIcon IconManager::GetSimpleIcon(IconType _type, const QColor& _color)
{
    return GetInstance().GetSimpleIcon_internal(_type, _color);
}
QIcon IconManager::GetSimpleIcon_internal(IconType _type, const QColor& _color)
{
    if (_type == IconType::None)
    {
        return QIcon();
    }

    QPixmap pixmap = QPixmap(myIconFolder + "TabIcon_" + myIconFileCharMap[_type] + ".png");
    QBitmap mask = pixmap.createMaskFromColor(QColorConstants::Transparent, Qt::MaskInColor);
    pixmap.fill(_color);
    pixmap.setMask(mask);

    return QIcon(pixmap);
}

/*
 *
 * pixmap = QPixmap(filename)
mask = pixmap.createMaskFromColor(QColor('black'), Qt.MaskOutColor) -> createMaskFromColor(QColor('transparent'), Qt.MaskInColor)
pixmap.fill((QColor('red')))
pixmap.setMask(mask)

btNew = QToolButton()
btNew.setIcon(QIcon(pixmap))

*/
