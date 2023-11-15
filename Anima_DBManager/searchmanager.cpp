#include "searchmanager.h"

#include <QDebug>


SearchManager::SearchManager() : dbManager{DB_Manager::GetDB_Manager()} {}

const SearchManager& SearchManager::GetInstance()
{
    static SearchManager instance = SearchManager();
    return instance;
}

QList<SearchResult> SearchManager::Search(const SearchParameter& _parameters) const
{
    QList<SearchResult> results = QList<SearchResult>();

    if (_parameters.mySearchOnStructs)
    {
        SearchStruct(_parameters, results);
    }
    if (_parameters.mySearchOnStrings)
    {
        SearchString(_parameters, results);
    }
    if (_parameters.mySearchOnEnum)
    {
        SearchEnum(_parameters, results);
    }

    return results;
}


void SearchManager::SearchStruct(const SearchParameter& _parameters, QList<SearchResult>& _outResults) const
{
    Q_ASSERT(_parameters.mySearchOnStructs);

    const int tableCount = dbManager.GetStructuresCount();
    for (int i = 0; i < tableCount; i++)
    {
        const StructureDB* structTable = dbManager.GetStructureTable(i);
        const QString& structTableName = structTable->GetTemplateName();
        const auto& structList = structTable->GetStructures();
        int j = 0;
        for (const auto* structure : structList)
        {
            int k = -1;
            for (const auto* attr : structure->GetAttributes())
            {
                k++;
                if (!_parameters.myAttributeIgnoreSearchMap[attr->GetType()])
                {
                    continue;
                }

                const QString attrVal = attr->GetValue_String();
                if (IsStringOk(attrVal, _parameters))
                {
                    // ADD to results
                    SearchResult result = SearchResult();
                    result.myCategory = 0;
                    result.myTableIndex = i;
                    result.myRowIndex = j;
                    result.myColIndex = k;
                    result.myDisplayString.append("STRUCT");
                    result.myDisplayString.append(structTableName);
                    result.myDisplayString.append(structTable->GetStructureRowName(j));
                    result.myDisplayString.append(attr->GetTemplate()->GetName());
                    result.myDisplayString.append(attrVal);

                    _outResults.append(result);
                }
            }
            j++;
        }
    }
}
void SearchManager::SearchString(const SearchParameter& _parameters, QList<SearchResult>& _outResults) const
{
    Q_ASSERT(_parameters.mySearchOnStrings);

    const int tableCount = dbManager.GetStringTableCount();
    for (int i = 0; i < tableCount; i++)
    {
        const SStringTable* sTable = dbManager.GetStringTable(i);
        const QString& sTableName = sTable->GetTableName();
        const auto& stringList = sTable->GetStringItems();
        int j = 0;
        for (const auto& item : stringList)
        {
            for (SStringHelper::SStringLanguages language = SStringHelper::SStringLanguages::French; language < SStringHelper::SStringLanguages::Count; SStringHelper::IncrementLanguage(language))
            {
                if (!_parameters.myLanguageIgnoreSearchMap[language])
                {
                    continue;
                }

                const QString& str = item.GetString(language);
                if (IsStringOk(str, _parameters))
                {
                    // ADD to results
                    SearchResult result = SearchResult();
                    result.myCategory = 1;
                    result.myTableIndex = i;
                    result.myRowIndex = j;
                    result.myColIndex = language;
                    result.myDisplayString.append("STRING");
                    result.myDisplayString.append(sTableName);
                    result.myDisplayString.append(item.GetIdentifier());
                    result.myDisplayString.append(SStringHelper::GetLanguageCD(language));
                    result.myDisplayString.append(str);

                    _outResults.append(result);
                }
            }
            j++;
        }
    }
}
void SearchManager::SearchEnum(const SearchParameter& _parameters, QList<SearchResult>& _outResults) const
{
    Q_ASSERT(_parameters.mySearchOnEnum);

    const int enumCount = dbManager.GetEnumCount();
    for (int i = 0; i < enumCount; i++)
    {
        const Enumerator* enumerator = dbManager.GetEnum(i);
        const int valueCount = enumerator->GetValueCount();
        const QString& enumName = enumerator->GetName();
        for (int j = 0; j < valueCount; j++)
        {
            const QString& val = enumerator->GetValue(j);
            if (IsStringOk(val, _parameters))
            {
                // ADD to results
                SearchResult result = SearchResult();
                result.myCategory = 2;
                result.myTableIndex = i;
                result.myRowIndex = j;
                result.myDisplayString.append("ENUM");
                result.myDisplayString.append(enumName);
                result.myDisplayString.append(val);
                result.myDisplayString.append("");
                result.myDisplayString.append("");

                _outResults.append(result);
            }
        }
    }
}

bool SearchManager::IsStringOk(const QString& _str, const SearchParameter& _parameters) const
{
    if (_parameters.myWholeWord)
    {
        for (const auto& word : _str.split(' ', Qt::SkipEmptyParts))
        {
            if (_parameters.mySearchedString.compare(word, _parameters.myCaseSensitivity) == 0)
            {
                return true;
            }
        }
    }
    else
    {
        if (_str.contains(_parameters.mySearchedString, _parameters.myCaseSensitivity))
        {
            return true;
        }
    }

    return false;
}
