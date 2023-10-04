#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QIcon>
#include <QMap>
#include <QString>


class IconManager
{
public:
    enum class IconType
    {
        Square = 0,
        SquareHollow,
        Circle,
        CircleHollow,
        TriangleUp,
        TriangleUpHollow,
        TriangleDown,
        TriangleDownHollow,
        Star,
        StarHollow,
        V,
        X,
        Anima,
        AnimaHollow,
        Stuff,
        StuffHollow,
        Map,
        MapHollow,
        Scroll,
        ScrollHollow,
        Face,
        FaceHollow,
        Text1,
        Text2,
        Text3,
        Text4,

        Count,
        None = Count
    };

private:
    const QString myIconFolder = "../icons/";
    QMap<IconType, QChar> myIconFileCharMap;
    IconManager();
    IconManager(IconManager& other) = delete;

    QIcon GetSimpleIcon_internal(IconType _type, const QColor& _color);

    static IconManager& GetInstance();

public:
    static QIcon GetAppIcon();
    static QIcon GetSimpleIcon(IconType _type, const QColor& _color);
};

#endif // ICONMANAGER_H
