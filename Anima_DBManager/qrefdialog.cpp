#include "qrefdialog.h"

#include <QPushButton>
#include <QVBoxLayout>

QRefDialog::QRefDialog(const StructureDB& structureDB, int& outIndex, QWidget* parent) :
    QDialog(parent),
    myIndex(outIndex),
    myStructureDB(structureDB)
{
    QVBoxLayout* vLayout = new QVBoxLayout();
    setLayout(vLayout);

    vLayout->addWidget(new QLabel("Choose an item from : " + myStructureDB.GetTemplateName()));

    myComboBox = new QComboBox();
    myComboBox->setEditable(true);
    vLayout->addWidget(myComboBox);
    myStructContent = new QLabel();
    vLayout->addWidget(myStructContent);

    const int structCount = myStructureDB.GetStructureCount();
    for (int i = 0; i < structCount; i++)
    {
        myComboBox->addItem(myStructureDB.GetStructureRowName(i));
    }
    QObject::connect(myComboBox, &QComboBox::currentIndexChanged, this, &QRefDialog::UpdateIndex);
    myComboBox->setCurrentIndex(myIndex);
    UpdateIndex(myIndex);

    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout(this);
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QDialog::accept);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);
}


void QRefDialog::UpdateIndex(int index)
{
    if (index < 0 || index >= myComboBox->count())   // As it's editable, an additional option is added to the combobox
    {
        myComboBox->setCurrentIndex(myIndex);
        return;
    }

    const auto* referencedStruct = myStructureDB.GetStructureAt(index);
    if(!referencedStruct)
    {
        myComboBox->setCurrentIndex(myIndex);
        return;
    }

    myIndex = index;
    myStructContent->setText(referencedStruct->GetDisplayText());
}
