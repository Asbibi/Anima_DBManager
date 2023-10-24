#include "qimportstructdialog.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "db_manager.h"
#include "structureimporthelper.h"


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
        QMessageBox::critical(0, "Error Reading file", file.errorString());
    }

    if (mySelectedFilePath.endsWith(".json"))
    {
        PerformImportJSON(file, _structTable, _overrideChoice);
    }
    else if (mySelectedFilePath.endsWith(".csv"))
    {
        PerformImportCSV(file, _structTable, _overrideChoice);
    }
    else
    {
        QMessageBox::warning(0, "Error Reading file", "File extension not handled.\nImport is aborted.");
    }

    file.close();
}
void QImportStructDialog::PerformImportJSON(QFile& _importedFile, StructureDB* _structTable, int _overrideChoice)
{
    const QJsonArray importedJsonsArray = QJsonDocument::fromJson(_importedFile.readAll()).array();
    _structTable->ReadValue_JSON_Table(importedJsonsArray, _overrideChoice);
}
void QImportStructDialog::PerformImportCSV(QFile& _importedFile, StructureDB* _structTable, int _overrideChoice)
{
    QTextStream in(&_importedFile);

    const int attrCount = _structTable->GetTemplate().GetAttributesCount();
    const int attrCountPlusKey = attrCount + 1;
    const QString& structAbbrev = _structTable->GetTemplateAbbrev();
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
                qWarning() << "CSV file import aborted because headers are not corresponding to the structure attribute : " << line;
                return;
            }
        }
        lineNumber++;

        QStringList fields;
        if (!StructureImportHelper::DecomposeCSVString(line, attrCountPlusKey, fields))
        {
            qWarning() << "Line " << lineNumber << " skipped because not formatted correctly : " << line;
            continue;
        }


        bool parsingOK = false;
        int structIndex = fields[0].remove(structAbbrev).toInt(&parsingOK);
        if (!parsingOK)
        {
            qWarning() << "Line " << lineNumber << " skipped because index couldn't be retrieved : " << line;
            continue;
        }
        fields.removeFirst();

        _structTable->ReadValue_CSV_Table(structIndex, fields, _overrideChoice);
    }
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
#ifdef CSV_EXPORT_ENABLED
                                                    "All Exports (*.json *.csv);;JSON (*.json);;CSV (*.csv)");
#else
                                                    "JSON (*.json)");
#endif

    if (fileName.isEmpty())
    {
        return;
    }

    mySelectedFilePath = fileName;
    bool tooLong = fileName.length() > 35;
    mySelectedFile->setText(tooLong ? "..." + fileName.right(32) : fileName);
}
