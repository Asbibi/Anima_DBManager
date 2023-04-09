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

bool QImportStructDialog::CheckCSVHearders(const StructureDB* _stringTable, const QString& _firstLine)
{
    Q_ASSERT(_stringTable != nullptr);
    QStringList headers = _firstLine.split(",");
    const auto& structTemplate = _stringTable->GetTemplate();
    const int attrCount = structTemplate.GetAttributesCount();
    if (headers.count() != (attrCount + 1))
    {
        return false;
    }

    for (int i = 0; i < attrCount; i++)
    {
        if (structTemplate.GetAttributeName(i) != headers[i+1])
        {
            return false;
        }
    }

    return true;
}


void QImportStructDialog::PerformImport(StructureDB* _structTable, int _overrideChoice)
{
    Q_ASSERT(_structTable != nullptr);
    qDebug() << "Import Struct Table";

    QFile file(mySelectedFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error Reading CSV file", file.errorString());
    }

    QTextStream in(&file);

    const int attrCount = _structTable->GetTemplate().GetAttributesCount();
    const int attrCountPlusKey = attrCount + 1;
    int lineNumber = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (lineNumber == 0)
        {
            if (CheckCSVHearders(_structTable, line))
            {
                lineNumber++;
                continue;
            }
            else
            {
                // display error message ? qWarning ?
                return;
            }
        }
        lineNumber++;

        int firstComma = line.indexOf(',');
        if (firstComma == -1)
        {
            qWarning() << "Line " << lineNumber << " skipped because not formatted correctly : " << line;
            continue;
        }

        line.replace(firstComma, 1, "\",");
        QStringList fields = line.split("\",\"");
        if (fields.count() != attrCountPlusKey)
        {
            qWarning() << "Line " << lineNumber << " skipped because not formatted correctly : " << line;
            continue;
        }

        fields[attrCount].remove(fields[attrCount].length()-1,1);

        //_stringTable->ImportString(language, fields[0], fields[1], _overrideChoice);
    }

    file.close();
}


void QImportStructDialog::OnApplyBtnClicked()
{
    if (mySelectedFilePath.isEmpty())
    {
        QDialog::reject();
        return;
    }

    int overrideChoice = myOverrideComboBox->currentIndex();
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    int structTableIndex = myTableComboBox->currentIndex();

    PerformImport(dbManager.GetStructureTable(structTableIndex), overrideChoice);

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
