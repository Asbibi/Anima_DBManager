#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H


class SearchManager
{
private:
    SearchManager();
    SearchManager(const SearchManager&) = delete;


public:
    static SearchManager& GetInstance();

    void Search();
};

#endif // SEARCHMANAGER_H
