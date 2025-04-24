#ifndef SSTRINGHELPER_H
#define SSTRINGHELPER_H

#include <QString>
#include <functional>

namespace SStringHelper {

enum SStringLanguages
{
    French = 0,
    English,

    Count
};

SStringLanguages GetLanguageFromCD(const QString& _cd);
QString GetLanguageString(SStringLanguages _language);
QString GetLanguageCD(SStringLanguages _language);
QString GetUniqueIdentifier(QString& _baseIdentifier, std::function<bool(const QString&)> _ValidateId, bool _noneAutorized);
void CleanStringForIdentifier(QString& _identifier);
bool IsNameOkForAttribute(const QString& _name);
void IncrementLanguage(SStringLanguages& _language);

void ReplaceDictionaryReferenceInString(QString& _string, SStringHelper::SStringLanguages _language);

const QString& GetEmptyStringReference();

}


#endif // SSTRINGHELPER_H
