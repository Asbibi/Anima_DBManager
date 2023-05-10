#include "sstringhelper.h"

#define CD_FR "FR"
#define CD_EN "EN"


namespace SStringHelper {


SStringLanguages GetLanguageFromCD(const QString& _cd)
{
    if (_cd == CD_FR)
        return SStringLanguages::French;
    else if (_cd == CD_EN)
        return SStringLanguages::English;

    return SStringLanguages::Count;
}
QString GetLanguageString(SStringLanguages _language)
{
    switch (_language)
    {
        case SStringLanguages::French:
            return "Français";
        case SStringLanguages::English:
            return "English";            
        default:
            break;
    }
    return "";
}
QString GetLanguageCD(SStringLanguages _language)
{
    switch (_language)
    {
        case SStringLanguages::French:
            return CD_FR;
        case SStringLanguages::English:
            return CD_EN;
        default:
            break;
    }
    return "";
}

QString GetUniqueIdentifier(QString& _baseIdentifier, std::function<bool(const QString&)> _ValidateId, bool _noneAutorized)
{
    if (_noneAutorized && _ValidateId(_baseIdentifier))
        return _baseIdentifier;

    int addition = 1;
    const int last_Index = _baseIdentifier.lastIndexOf('_');
    if (last_Index >= 0)
    {
        const QString post_String = _baseIdentifier.right(_baseIdentifier.length() - last_Index - 1);
        bool isOnlyDigit;
        int baseAddition = post_String.toInt(&isOnlyDigit);
        if (isOnlyDigit)
        {
            addition = baseAddition;
            _baseIdentifier = _baseIdentifier.left(last_Index);
        }
    }

    QString identifier = _baseIdentifier + '_' + QString::number(addition);
    while (!_ValidateId(identifier))
    {
        addition++;
        identifier = _baseIdentifier + '_' + QString::number(addition);
    }

    return identifier;
}

}
