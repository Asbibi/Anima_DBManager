#ifndef SSTRINGHELPER_H
#define SSTRINGHELPER_H

#include <QString>
#include <functional>

namespace SStringHelper {

QString GetUniqueIdentifier(QString& _baseIdentifier, std::function<bool(const QString&)> _ValidateId, bool _noneAutorized);
void CleanStringForIdentifier(QString& _identifier);
bool IsNameOkForAttribute(const QString& _name);

void ReplaceDictionaryReferenceInString(QString& _string, int _languageIndex);

const QString& GetEmptyStringReference();

}


#endif // SSTRINGHELPER_H
