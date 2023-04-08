#include "qimportstringdialog.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "db_manager.h"


QImportStringDialog::QImportStringDialog(QPanelString* _stringWidget, QWidget* _parent) :
    QDialog(_parent),
    myStringWidget(_stringWidget)
{
    Q_ASSERT(_stringWidget != nullptr);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const auto& dbManager = DB_Manager::GetDB_Manager();
    myTableComboBox = new QComboBox();
    int stringTableCount = dbManager.GetStringTableCount();
    for (int i = 0; i < stringTableCount; i++)
    {
        myTableComboBox->addItem(dbManager.GetStringTable(i)->GetTableName());
    }
    myTableComboBox->addItem("*** NEW ***");
    myTableComboBox->setCurrentIndex(stringTableCount);
    QObject::connect(myTableComboBox, &QComboBox::currentIndexChanged, this, &QImportStringDialog::OnTableComboBoxChanged);
    vLayout->addWidget(myTableComboBox);



    QWidget* fileWidget = new QWidget();
    QGridLayout* gLayoutFile = new QGridLayout();
    fileWidget->setLayout(gLayoutFile);
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        SStringHelper::SStringLanguages language = (SStringHelper::SStringLanguages)i;
        gLayoutFile->addWidget(new QLabel(SStringHelper::GetLanguageString(language) + " :"), i, 0);
        auto* fileLabel = new QLabel();
        fileLabel->setMinimumWidth(200);
        gLayoutFile->addWidget(fileLabel, i, 1);
        auto* fileBtn = new QPushButton("+");
        fileBtn->setMaximumWidth(20);
        QObject::connect(fileBtn, &QPushButton::clicked, this, [this, language, fileLabel]{OnFileBtnClicked(language, fileLabel);});
        gLayoutFile->addWidget(fileBtn, i, 2, Qt::AlignmentFlag::AlignCenter);
    }
    vLayout->addWidget(fileWidget);

    QFormLayout* fLayout = new QFormLayout();
    myOverrideComboBox = new QComboBox();
    myOverrideComboBox->addItem("Overwrite with Imported");
    myOverrideComboBox->addItem("Keep existing");
    myOverrideComboBox->addItem("Import with new name");
    myOverrideComboBox->setEnabled(false);
    fLayout->addRow("On confict : ", myOverrideComboBox);
    myNewTableName = new QLineEdit("NewStringTable");
    fLayout->addRow("Name of new Table : ", myNewTableName);
    vLayout->addLayout(fLayout);

    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QImportStringDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    setWindowTitle("Import CSV files as String Table");
}


void QImportStringDialog::OnApplyBtnClicked()
{
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    int stringTableIndex = myTableComboBox->currentIndex();
    if (stringTableIndex == dbManager.GetStringTableCount())
    {
        dbManager.AddStringTable(myNewTableName->text());
        myStringWidget->UpdateItemList();
    }

    qDebug() << "Import String Table";

    QDialog::accept();
}

void QImportStringDialog::OnFileBtnClicked(SStringHelper::SStringLanguages _language, QLabel* _label)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV String Table file",
                                                    DB_Manager::GetDB_Manager().GetProjectContentFolderPath(),
                                                    "CSV (*.csv)");

    if (fileName.isEmpty())
    {
        return;
    }

    myCSVMap.insert(_language, fileName);

    bool tooLong = fileName.length() > 35;
    _label->setText(tooLong ? "..." + fileName.right(32) : fileName);
}

void QImportStringDialog::OnTableComboBoxChanged(int _index)
{
    bool newTable = _index == (myTableComboBox->count() -1);
    myNewTableName->setEnabled(newTable);
    myOverrideComboBox->setEnabled(!newTable);
}
