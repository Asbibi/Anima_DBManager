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

QString GetLanguageString(SStringLanguages _language);
QString GetUniqueIdentifier(QString& _baseIdentifier, std::function<bool(const QString&)> _ValidateId, bool _noneAutorized);

}


#endif // SSTRINGHELPER_H
