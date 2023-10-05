#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include "db_manager.h"
#include "searchparameter.h"
#include "searchresult.h"
#include <QStringList>

class SearchManager
{
private:
    const DB_Manager& dbManager;

    SearchManager();
    SearchManager(const SearchManager&) = delete;

    void SearchStruct(const SearchParameter& _parameters, QList<SearchResult>& _outResults);
    void SearchString(const SearchParameter& _parameters, QList<SearchResult>& _outResults);
    void SearchEnum(const SearchParameter& _parameters, QList<SearchResult>& _outResults);

public:
    static SearchManager& GetInstance();

    QList<SearchResult> Search(const SearchParameter& _parameters);
};

#endif // SEARCHMANAGER_H
