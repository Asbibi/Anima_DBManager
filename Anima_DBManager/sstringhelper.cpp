#include "sstringhelper.h"

#include "db_manager.h"
#include <QList>

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
void CleanStringForIdentifier(QString& _identifier)
{
    static const QList<QChar> bannedChars = {'\"',
                                             '\'',
                                             '=',
                                             '(',
                                             ')',
                                             '[',
                                             ']',
                                             '{',
                                             '}',
                                             '+',
                                             '-',
                                             '*',
                                             '/',
                                             '!',
                                             '?',
                                             ',',
                                             ';',
                                             ':',
                                             '#',
                                             '%',
                                             '$',
                                             '&',
                                             '<',
                                             '>'};
    _identifier.replace(' ','_');
    for (const auto& bc : bannedChars)
    {
        _identifier.remove(bc);
    }
}
bool IsNameOkForAttribute(const QString& _name)
{
    return _name != "" && DB_Manager::GetDB_Manager().GetAttributeFullName(_name) != "Name";
}
void IncrementLanguage(SStringLanguages& _language)
{
    int l = _language;
    l++;
    _language = (SStringLanguages)l;
}


void ReplaceDictionaryReferenceInString(QString& _string, SStringHelper::SStringLanguages _language)
{
    static QRegularExpression dictionaryIdRegex(R"(\$(.*?)\$)");

    const SStringTable* dictionary = DB_Manager::GetDB_Manager().GetDictionary();
    Q_ASSERT(dictionary != nullptr);

    QString copy = _string;
    QRegularExpressionMatchIterator matchIterator = dictionaryIdRegex.globalMatch(copy);
    while (matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        QString indicator = match.captured(1); // Extracts text between '$'
        const QString* dictionaryString = dictionary->GetString(indicator, _language);
        _string.replace(match.captured(0), dictionaryString != nullptr ? *dictionaryString : "");
    }
}

const QString& GetEmptyStringReference()
{
    static QString emptyStringRef = "";
    return emptyStringRef;
}

}
