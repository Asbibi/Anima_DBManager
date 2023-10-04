#ifndef QPANELWITHCOUNT_H
#define QPANELWITHCOUNT_H

#include "qpanelbase.h"

#include <QFormLayout>
#include <QSpinBox>


class QPanelWithCount : public QPanelBase
{
    Q_OBJECT

protected:
    int myCachedStructureCount;
    QSpinBox* myRowCountSpinner;

    void InitItemCountWidget(QFormLayout* _editLayout);
    virtual int RefreshItemCount_Internal(const int _tableIndex) = 0;
    virtual void SetItemCount_Internal(const int _tableIndex, const int _newCount) = 0;

public:
    explicit QPanelWithCount(const QString& itemBoxName, bool _uniqueNames, QWidget* _parent = nullptr);
    void RefreshItemCount(const int _tableIndex);

public slots:
    void SetItemCount();

};

#endif // QPANELWITHCOUNT_H
