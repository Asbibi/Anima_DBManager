#ifndef QPANELSEARCH_H
#define QPANELSEARCH_H

#include <QWidget>

#include "searchparameter.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
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
    QTableWidget* myResultTable;
    QLabel* myResultCount;

    SearchParameter mySearchParameters;


public:
    explicit QPanelSearch(QWidget* _parent = nullptr);

public slots:
    void OnSearchedStringChanged(const QString& _str);
    void OnStructCheckboxChanged(int _state);
    void OnStringCheckboxChanged(int _state);
    void OnEnumCheckboxChanged(int _state);
    void OnAttributeCheckBoxChanged(int _state, AttributeTypeHelper::Type _type);
    void OnLanguageCheckBoxChanged(int _state, SStringHelper::SStringLanguages _language);
    void OnCaseCheckboxChanged(int _state);
    void OnWholeCheckboxChanged(int _state);

    void OnSearchRequested();
};

#endif // QPANELSEARCH_H
