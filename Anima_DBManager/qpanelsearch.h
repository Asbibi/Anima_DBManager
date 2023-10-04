#ifndef QPANELSEARCH_H
#define QPANELSEARCH_H

#include <QWidget>

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
    QGroupBox* mySearchOnAttributGroup;
    QCheckBox* mySearchOnString;
    QCheckBox* mySearchOnEnum;
    QTableWidget* myResultTable;
    QLabel* myResultCount;



public:
    explicit QPanelSearch(QWidget* _parent = nullptr);

signals:
public slots:
    void OnStructCheckboxChanged(int _state);

};

#endif // QPANELSEARCH_H
