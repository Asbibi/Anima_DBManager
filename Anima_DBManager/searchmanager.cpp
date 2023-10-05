#include "searchmanager.h"

SearchManager::SearchManager() {}

SearchManager& SearchManager::GetInstance()
{
    static SearchManager instance = SearchManager();
    return instance;
}
