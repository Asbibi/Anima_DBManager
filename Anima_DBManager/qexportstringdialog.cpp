#include "qexportstringdialog.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>

#include "db_manager.h"
#include "qgridlayouthelper.h"


QExportStringDialog::QExportStringDialog(bool _userSelectTables, QWidget* _parent) :
    QDialog(_parent),
    myUserSelectTables(_userSelectTables)
{
    if (!_userSelectTables)
    {
        int stringTableCount = DB_Manager::GetDB_Manager().GetStringTableCount();
        for (int i = 0; i < stringTableCount; i++)
        {
            myTablesToExportList.insert(i);
        }
    }

    InitWidget();
}
QExportStringDialog::QExportStringDialog(int _tableToImport, QWidget* _parent) :
    QDialog(_parent),
    myUserSelectTables(false)
{
    Q_ASSERT(0 <= _tableToImport && _tableToImport < DB_Manager::GetDB_Manager().GetStringTableCount());
    myTablesToExportList.insert(_tableToImport);

    InitWidget();
}
void QExportStringDialog::InitWidget()
{
    const auto& dbManager = DB_Manager::GetDB_Manager();

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    QFormLayout* fLayout = new QFormLayout();

    QVBoxLayout* nameLayout = new QVBoxLayout();
    myExportNameTemplateText = new QLineEdit("ST_#CD#_#ST#");
    QObject::connect(myExportNameTemplateText, &QLineEdit::editingFinished, this, &QExportStringDialog::UpdateApplyButtonEnable);
    nameLayout->addWidget(myExportNameTemplateText);
    nameLayout->addWidget(new QLabel("Use <font color='blue'>\"#CD#\"</font> as a language abbreviation placeholder."));
    nameLayout->addWidget(new QLabel("Use <font color='blue'>\"#ST#\"</font> as a String Table name placeholder."));
    fLayout->addRow("Export Name :", nameLayout);

    QHBoxLayout* fileLayout = new QHBoxLayout();
    myExportFolderText = new QLabel();
    SetDirPath(dbManager.GetProjectContentFolderPath());
    myExportFolderText->setMinimumWidth(200);
    fileLayout->addWidget(myExportFolderText);
    auto* fileBtn = new QPushButton("...");
    fileBtn->setMaximumWidth(20);
    QObject::connect(fileBtn, &QPushButton::clicked, this, &QExportStringDialog::OnDirBtnClicked);
    fileLayout->addWidget(fileBtn);
    fLayout->addRow("Export Directory :", fileLayout);

    QGridLayout* gLayoutLanguage = new QGridLayout();
    const auto& languages = dbManager.GetLanguages();
    const int languagesCount = languages.GetLanguageCount();
    for (int i = 0; i < languagesCount; i++)
    {
        QCheckBox* checkBox = new QCheckBox(languages.GetLanguage(i).GetName());
        QObject::connect(checkBox, &QCheckBox::checkStateChanged, this, [this, i](Qt::CheckState _state){ OnLanguageChecked(_state, i); });
        auto gridIndexes = QGridLayoutHelper::GetGridIndexes(i);
        gLayoutLanguage->addWidget(checkBox, gridIndexes.row, gridIndexes.col);
    }
    fLayout->addRow("Languages :", gLayoutLanguage);

    if (myUserSelectTables)
    {
        QGridLayout* gLayoutTable = new QGridLayout();
        const int tableCount = dbManager.GetStringTableCount();
        for (int i = 0; i < tableCount; i++)
        {
            QCheckBox* checkBox = new QCheckBox(dbManager.GetStringTable(i)->GetTableName());
            QObject::connect(checkBox, &QCheckBox::checkStateChanged, this, [this, i](Qt::CheckState _state){ OnTableChecked(_state, i); });
            auto gridIndexes = QGridLayoutHelper::GetGridIndexes(i);
            gLayoutTable->addWidget(checkBox, gridIndexes.row, gridIndexes.col);
        }
        fLayout->addRow("Tables :", gLayoutTable);
    }

    QComboBox* conflictPolicyBox = new QComboBox();
    conflictPolicyBox->addItem("Overwrite");
    conflictPolicyBox->addItem("Ignore");
    conflictPolicyBox->addItem("Ask on conflict");
    QObject::connect(conflictPolicyBox, &QComboBox::currentIndexChanged, this,  &QExportStringDialog::OnConflictPolicyChanged);
    conflictPolicyBox->setCurrentIndex(0);
    fLayout->addRow("Conflict policy :", conflictPolicyBox);

    vLayout->addLayout(fLayout);


    QWidget* btnWidget = new QWidget();
    myApplyBtn = new QPushButton("Export");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(myApplyBtn);
    QObject::connect(myApplyBtn, &QPushButton::clicked, this, &QExportStringDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    myExportErrorLabel = new QLabel();
    vLayout->addWidget(myExportErrorLabel);

    setWindowTitle("Export String Table as CSV file");

    UpdateApplyButtonEnable();
}
void QExportStringDialog::SetDirPath(const QString& _path)
{
    if (_path.isEmpty())
    {
        return;
    }
    static const QString dirPathTemplate = "<font color='green'>%1</font>";
    myExportFolder = _path;
    myExportFolderText->setText(dirPathTemplate.arg(_path));
}


ExportError QExportStringDialog::CanApply() const
{
    int tableCount = myTablesToExportList.size();
    int languageCount = myLanguageToExportList.size();
    if (tableCount == 0 || languageCount == 0)
        return ExportError::NOTHING_TO_EXPORT;

    QString exportTemplate = myExportNameTemplateText->text();
    if (exportTemplate.isEmpty())
        return ExportError::NO_EXPORT_NAME;

    if (myExportFolder.isEmpty() || !QDir(myExportFolder).exists())
        return ExportError::NO_EXPORT_FOLDER;

    if (languageCount > 1 && !exportTemplate.contains("#CD#"))
        return ExportError::NO_LANGUAGE_DIFF;


    if (tableCount > 1 && !exportTemplate.contains("#ST#"))
        return ExportError::NO_TABLE_DIFF;

    return ExportError::OK;
}
void QExportStringDialog::UpdateApplyButtonEnable()
{
    ExportError canApplyResult = CanApply();
    myApplyBtn->setEnabled(canApplyResult == ExportError::OK);
    switch(canApplyResult)
    {
    case ExportError::NO_EXPORT_NAME:
    {
        myExportErrorLabel->setVisible(true);
        static const QString langErrorText = "<font color='red'>Can't export : An export name is required to export.</font>";
        myExportErrorLabel->setText(langErrorText);
        break;
    }
    case ExportError::NO_EXPORT_FOLDER:
    {
        myExportErrorLabel->setVisible(true);
        static const QString langErrorText = "<font color='red'>Can't export : Select a valid folder to export to.</font>";
        myExportErrorLabel->setText(langErrorText);
        break;
    }
    case ExportError::NO_LANGUAGE_DIFF:
    {
        myExportErrorLabel->setVisible(true);
        static const QString langErrorText = "<font color='red'>Can't export : Use \"#CD#\" in the export name or select only one Language.</font>";
        myExportErrorLabel->setText(langErrorText);
        break;
    }
    case ExportError::NO_TABLE_DIFF:
    {
        myExportErrorLabel->setVisible(true);
        static const QString tableErrorText = "<font color='red'>Can't export : Use \"#ST#\" in the export name or select only one String Table.</font>";
        myExportErrorLabel->setText(tableErrorText);
        break;
    }
    default:
    {
        myExportErrorLabel->setVisible(false);
        break;
    }
    }
}



const QSet<int>& QExportStringDialog::GetTablesToExport() const
{
    return myTablesToExportList;
}
const QSet<int>& QExportStringDialog::GetLanguagesToExport() const
{
    return myLanguageToExportList;
}
const QString& QExportStringDialog::GetExportFolderPath() const
{
    return myExportFolder;
}
QString QExportStringDialog::GetExportFileNameTemplate() const
{
    return myExportNameTemplateText->text();
}
QExportStringConflictPolicy QExportStringDialog::GetConflictPolicy() const
{
    return myConflictPolicy;
}


void QExportStringDialog::OnApplyBtnClicked()
{
    Q_ASSERT(CanApply() == ExportError::OK);


    QDialog::accept();
}
void QExportStringDialog::OnLanguageChecked(Qt::CheckState _state, int _languageIndex)
{
    Q_ASSERT(_state != Qt::PartiallyChecked);
    if (_state == Qt::Unchecked)
    {
        myLanguageToExportList.remove(_languageIndex);
    }
    else
    {
        myLanguageToExportList.insert(_languageIndex);
    }

    UpdateApplyButtonEnable();
}
void QExportStringDialog::OnTableChecked(Qt::CheckState _state, int _tableIndex)
{
    Q_ASSERT(myUserSelectTables);
    Q_ASSERT(_state != Qt::PartiallyChecked);
    if (_state == Qt::Unchecked)
    {
        myTablesToExportList.remove(_tableIndex);
    }
    else
    {
        myTablesToExportList.insert(_tableIndex);
    }

    UpdateApplyButtonEnable();
}
void QExportStringDialog::OnDirBtnClicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Select Folder to export String Tables :",
                                                        myExportFolder);
    SetDirPath(dirPath);
    UpdateApplyButtonEnable();
}
void QExportStringDialog::OnConflictPolicyChanged(int _policyIndex)
{
    switch(_policyIndex)
    {
    case 0:
        myConflictPolicy = QExportStringConflictPolicy::OVERWRITE;
        break;
    case 1:
        myConflictPolicy = QExportStringConflictPolicy::IGNORE;
        break;
    default:
    case 2:
        myConflictPolicy = QExportStringConflictPolicy::ASK;
        break;
    }
}