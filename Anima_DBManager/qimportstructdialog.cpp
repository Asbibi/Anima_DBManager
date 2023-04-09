#include "qimportstructdialog.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "db_manager.h"


QImportStructDialog::QImportStructDialog(QPanelStruct* _structWidget, QWidget* _parent) :
    QDialog(_parent),
    mySelectedFilePath(""),
    myStructWidget(_structWidget)
{
    Q_ASSERT(myStructWidget != nullptr);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const auto& dbManager = DB_Manager::GetDB_Manager();
    myTableComboBox = new QComboBox();
    int structTableCount = dbManager.GetStructuresCount();
    for (int i = 0; i < structTableCount; i++)
    {
        myTableComboBox->addItem(dbManager.GetStructureTable(i)->GetTemplateName());
    }
    vLayout->addWidget(myTableComboBox);


    QFormLayout* fLayout = new QFormLayout();
    QHBoxLayout* hLayoutOpen = new QHBoxLayout();
    mySelectedFile = new QLabel();
    mySelectedFile->setMinimumWidth(200);
    hLayoutOpen->addWidget(mySelectedFile);
    auto* fileBtn = new QPushButton("+");
    fileBtn->setMaximumWidth(20);
    QObject::connect(fileBtn, &QPushButton::clicked, this, &QImportStructDialog::OnFileBtnClicked);
    hLayoutOpen->addWidget(fileBtn);
    fLayout->addRow("Open file :", hLayoutOpen);
    myOverrideComboBox = new QComboBox();
    myOverrideComboBox->addItem("Overwrite with Imported");
    myOverrideComboBox->addItem("Keep existing");
    myOverrideComboBox->addItem("Import with new name (/!\\)");
    fLayout->addRow("On confict :", myOverrideComboBox);
    vLayout->addLayout(fLayout);


    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QImportStructDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    if (structTableCount == 0)
    {
        applyBtn->setEnabled(false);
    }

    setWindowTitle("Import CSV files as Struct Table");
}

int QImportStructDialog::GetTableIndex() const
{
    return myTableComboBox->currentIndex();
}


/*
void QImportStuctDialog::PerformImport(SStringTable* _stringTable, int _overrideChoice)
{
    Q_ASSERT(_stringTable != nullptr);
    qDebug() << "Import String Table";
    for (SStringHelper::SStringLanguages language : myCSVMap.keys())
    {
        QFile file(myCSVMap[language]);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "Error Reading CSV file", file.errorString());
        }

        QTextStream in(&file);

        int lineNumber = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            if (lineNumber == 0)
            {
                lineNumber++;
                continue;
            }
            lineNumber++;

            QStringList fields = line.split("\",\"");
            if (fields.count() != 2)
            {
                qWarning() << "Line " << lineNumber << " skipped because not formatted correctly : " << line;
                continue;
            }

            fields[0].remove(0,1);
            fields[1].remove(fields[1].length()-1,1);
            if (fields[1].isEmpty())
            {
                continue;
            }
            qDebug() << "Importing language: " << SStringHelper::GetLanguageCD(language) << "key: " << fields[0] << " value: " << fields[1];

            _stringTable->ImportString(language, fields[0], fields[1], _overrideChoice);
        }

        file.close();
    }
}
*/

void QImportStructDialog::OnApplyBtnClicked()
{
    if (mySelectedFilePath.isEmpty())
    {
        QDialog::reject();
        return;
    }
/*
    int overrideChoice = myOverrideComboBox->currentIndex();
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    int stringTableIndex = myTableComboBox->currentIndex();
    bool newTable = false;
    if (stringTableIndex == dbManager.GetStringTableCount())
    {
        dbManager.AddStringTable(myNewTableName->text());
        overrideChoice = 0;
        newTable = true;
        dbManager.GetStringTable(stringTableIndex)->RemoveStringItem(0);
    }

    PerformImport(dbManager.GetStringTable(stringTableIndex), overrideChoice);

    if (newTable)
    {
        if (dbManager.GetStringTable(stringTableIndex)->GetStringItemCount() == 0)
        {
            dbManager.RemoveStringTable(stringTableIndex);
        }
        else
        {
            myStringWidget->UpdateItemList();
        }
    }
*/
    QDialog::accept();
}

void QImportStructDialog::OnFileBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV Struct Table file",
                                                    DB_Manager::GetDB_Manager().GetProjectContentFolderPath(),
                                                    "CSV (*.csv)");

    if (fileName.isEmpty())
    {
        return;
    }

    mySelectedFilePath = fileName;
    bool tooLong = fileName.length() > 35;
    mySelectedFile->setText(tooLong ? "..." + fileName.right(32) : fileName);
}
