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

    QLineEdit* mySearchField;
    QCheckBox* mySearchOnStruct;
    QGroupBox* mySearchOnAttributeGroup;
    QCheckBox* mySearchOnString;
    QGroupBox* mySearchOnLanguageGroup;
    QCheckBox* mySearchOnEnum;
    QTableWidget* myResultTable;
    QLabel* myResultCount;

    SearchParameter mySearchParameters;


public:
    explicit QPanelSearch(QWidget* _parent = nullptr);

public slots:
    void OnStructCheckboxChanged(int _state);
    void OnStringCheckboxChanged(int _state);
    void OnEnumCheckboxChanged(int _state);
    void OnAttributeCheckBoxChanged(int _state, AttributeTypeHelper::Type _type);
    void OnLanguageCheckBoxChanged(int _state, SStringHelper::SStringLanguages _language);
};

#endif // QPANELSEARCH_H
