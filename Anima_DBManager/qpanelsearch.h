#ifndef QPANELSEARCH_H
#define QPANELSEARCH_H

#include <QWidget>

#include "searchparameter.h"
#include "searchresult.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

class QPanelSearch : public QWidget
{
    Q_OBJECT

private:
    static const QString ourCountString;

    QCheckBox* mySearchOnStruct;
    QGroupBox* mySearchOnAttributeGroup;
    QCheckBox* mySearchOnString;
    QGroupBox* mySearchOnLanguageGroup;
    QGridLayout* myLanguageGroupLayout;
    QPushButton* mySearchBtn;
    QTableWidget* myResultTable;
    QLabel* myResultCount;

    SearchParameter mySearchParameters;
    QList<SearchResult> myCurrentSearchResults;

    void UpdateLanguageFilterWidget();
    void UpdateWidgetsWithSearchResults();


public:
    explicit QPanelSearch(QWidget* _parent = nullptr);

public slots:
    void OnLanguageChanged();

    void OnSearchedStringChanged(const QString& _str);
    void OnStructCheckboxChanged(int _state);
    void OnStringCheckboxChanged(int _state);
    void OnEnumCheckboxChanged(int _state);
    void OnAttributeCheckBoxChanged(int _state, AttributeTypeHelper::Type _type);
    void OnLanguageCheckBoxChanged(int _state, int _languageIndex);
    void OnCaseCheckboxChanged(int _state);
    void OnWholeCheckboxChanged(int _state);

    void OnSearchRequested();

    void OnSearchResultDoubleClicked(int _row, int _col);
};

#endif // QPANELSEARCH_H
