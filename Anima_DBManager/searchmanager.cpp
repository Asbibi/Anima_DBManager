#include "searchmanager.h"

SearchManager::SearchManager() : dbManager{DB_Manager::GetDB_Manager()} {}

SearchManager& SearchManager::GetInstance()
{
    static SearchManager instance = SearchManager();
    return instance;
}

QList<SearchResult> SearchManager::Search(const SearchParameter& _parameters)
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


void SearchManager::SearchStruct(const SearchParameter& _parameters, QList<SearchResult>& _outResults)
{

}
void SearchManager::SearchString(const SearchParameter& _parameters, QList<SearchResult>& _outResults)
{

}
void SearchManager::SearchEnum(const SearchParameter& _parameters, QList<SearchResult>& _outResults)
{
    const int enumCount = dbManager.GetEnumCount();
    for (int i = 0; i < enumCount; i++)
    {
        const Enumerator* enumerator = dbManager.GetEnum(i);
        const int valueCount = enumerator->GetValueCount();
        const QString& enumName = enumerator->GetName();
        for (int j = 0; j < valueCount; j++)
        {
            const QString& val = enumerator->GetValue(j);
            if (_parameters.myWholeWord)
            {
                for (const auto& word : val.split(' ', Qt::SkipEmptyParts))
                {
                    if (_parameters.mySearchedString.compare(word, _parameters.myCaseSensitivity) == 0)
                    {
                        // ADD to results
                        SearchResult result = SearchResult();
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
            else
            {
                if (val.contains(_parameters.mySearchedString, _parameters.myCaseSensitivity))
                {
                    // ADD to results
                    SearchResult result = SearchResult();
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
}
