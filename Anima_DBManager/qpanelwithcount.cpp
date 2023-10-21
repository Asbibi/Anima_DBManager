#include "qpanelwithcount.h"

#include "db_manager.h"
#include <QMessageBox>

QPanelWithCount::QPanelWithCount(const QString& itemBoxName, bool _uniqueNames, QWidget *_parent) :
    QPanelBase(itemBoxName, _uniqueNames, _parent)
{}


void QPanelWithCount::InitItemCountWidget(QFormLayout* _editLayout)
{
    QHBoxLayout* numLayout = new QHBoxLayout();
    myRowCountSpinner = new QSpinBox();
    numLayout->addWidget(myRowCountSpinner);
    QPushButton* myPushBtn = new QPushButton("Set Item Count");
    myPushBtn->setMaximumWidth(90);
    numLayout->addWidget(myPushBtn);
    _editLayout->addRow("Count:", numLayout);

    QObject::connect(myPushBtn, &QPushButton::clicked, this, &QPanelWithCount::SetItemCount);
}


void QPanelWithCount::RefreshItemCount(const int _tableIndex)
{
    myCachedStructureCount = RefreshItemCount_Internal(_tableIndex);
    myRowCountSpinner->setValue(myCachedStructureCount);
}
void QPanelWithCount::SetItemCount()
{
    const int count = myRowCountSpinner->value();
    if (count < myCachedStructureCount)
    {
        QString dialogText = QString("You asked to set the item count to %1, which is less than current %2.\nProceeding will result into data suppression.\n\nProceed ?").arg(count).arg(myCachedStructureCount);
        QMessageBox::StandardButton btn = QMessageBox::warning(this, "Confirm new Item Count", dialogText, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (btn == QMessageBox::No)
        {
            return;
        }
    }
    SetItemCount_Internal(GetSelectedItem(), count);
    myCachedStructureCount = count;
}
