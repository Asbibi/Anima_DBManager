#include "qimportenumfromtextdialog.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "db_manager.h"


QImportEnumFromTextDialog::QImportEnumFromTextDialog(QWidget* _parent) :
    QDialog(_parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const auto& dbManager = DB_Manager::GetDB_Manager();
    myEnumComboBox = new QComboBox();
    int enumTableCount = dbManager.GetEnumCount();
    for (int i = 0; i < enumTableCount; i++)
    {
        myEnumComboBox->addItem(dbManager.GetEnum(i)->GetName());
    }
    myEnumComboBox->addItem("*** NEW ***");
    myEnumComboBox->setCurrentIndex(enumTableCount);
    QObject::connect(myEnumComboBox, &QComboBox::currentIndexChanged, this, &QImportEnumFromTextDialog::OnEnumComboBoxChanged);
    vLayout->addWidget(myEnumComboBox);

    QFormLayout* fLayout = new QFormLayout();
    myNewEnumName = new QLineEdit("NewEnumerator");
    fLayout->addRow("Name of new Enumerator : ", myNewEnumName);
    vLayout->addLayout(fLayout);

    myTextEdit = new QTextEdit();
    vLayout->addWidget(myTextEdit);

    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QImportEnumFromTextDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    setWindowTitle("Import Text as Enumerator");
}

void QImportEnumFromTextDialog::OnApplyBtnClicked()
{
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    int enumIndex = myEnumComboBox->currentIndex();
    bool newEnum = enumIndex == dbManager.GetEnumCount();

    if (newEnum)
    {
        dbManager.AddEnum(Enumerator(myNewEnumName->text()));
    }
    dbManager.AddValuesToEnum(enumIndex, myTextEdit->toPlainText());

    QDialog::accept();
}

void QImportEnumFromTextDialog::OnEnumComboBoxChanged(int _index)
{
    bool newEnum = _index == (myEnumComboBox->count() -1);
    myNewEnumName->setEnabled(newEnum);
}
