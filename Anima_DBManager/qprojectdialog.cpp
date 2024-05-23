#include "qprojectdialog.h"

#include "db_manager.h"
#include "sstringhelper.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>


const QString QProjectDialog::ourOriginalAttributeName = "AttributeName";
const QString QProjectDialog::ourPrefixColor = QColorConstants::DarkBlue.name();
const QString QProjectDialog::ourSuffixColor = QColorConstants::DarkGreen.name();


QProjectDialog::QProjectDialog(QWidget* _parent) :
    QDialog(_parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const QString titleStyle = "font-weight: bold";

    // Project Path
    auto* pathTitle = new QLabel("Unreal Project (Content folder) Path :");
    pathTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(pathTitle);
    vLayout->addSpacing(3);
    const auto& dbManager = DB_Manager::GetDB_Manager();
    myProjectPath = new QLabel();
    SetPath(dbManager.GetProjectContentFolderPath(false));
    QPushButton* changeBtn = new QPushButton("Select Directory");
    QObject::connect(changeBtn, &QPushButton::clicked, this, &QProjectDialog::OnSelectPath);
    QPushButton* resetBtn = new QPushButton("Clear");
    QObject::connect(resetBtn, &QPushButton::clicked, this, &QProjectDialog::OnResetPath);
    vLayout->addWidget(myProjectPath);
    QHBoxLayout* pathtBtnLayout = new QHBoxLayout();
    pathtBtnLayout->addWidget(changeBtn);
    pathtBtnLayout->addWidget(resetBtn);
    vLayout->addLayout(pathtBtnLayout);

    vLayout->addSpacing(6);
    auto* fixTitle = new QLabel("Attribute Prefix/Suffix:");
    fixTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(fixTitle);
    vLayout->addSpacing(3);
    QHBoxLayout* fixLayout = new QHBoxLayout();
    myPrefixEdit = new QLineEdit(dbManager.GetAttributePrefix());
    mySuffixEdit = new QLineEdit(dbManager.GetAttributeSuffix());
    myFixResult = new QLabel();
    myPrefixEdit->setStyleSheet("color: " + ourPrefixColor);
    mySuffixEdit->setStyleSheet("color: " + ourSuffixColor);
    QObject::connect(myPrefixEdit, &QLineEdit::textEdited, this, &QProjectDialog::OnPrefixChange);
    QObject::connect(mySuffixEdit, &QLineEdit::textEdited, this, &QProjectDialog::OnSuffixChange);
    fixLayout->addWidget(myPrefixEdit);
    fixLayout->addWidget(mySuffixEdit);
    vLayout->addLayout(fixLayout);
    vLayout->addWidget(myFixResult);
    UpdateFixResult();

    vLayout->addSpacing(6);
    auto* autoSaveTitle = new QLabel("Automatic Save");
    autoSaveTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(autoSaveTitle);
    vLayout->addSpacing(3);
    myAutoSaveEnable = new QCheckBox();
    myAutoSaveInterval = new QSpinBox();
    QFormLayout* autoSaveLayout = new QFormLayout();
    QObject::connect(myAutoSaveEnable, &QCheckBox::toggled, myAutoSaveInterval, &QWidget::setEnabled);
    autoSaveLayout->addRow("Enable AutoSave:", myAutoSaveEnable);
    autoSaveLayout->addRow("AutoSave Interval (minuts):", myAutoSaveInterval);
    vLayout->addLayout(autoSaveLayout);
    const bool enabled = dbManager.GetAutoSaveEnabled();
    myAutoSaveEnable->setChecked(enabled);
    myAutoSaveInterval->setEnabled(enabled);
    myAutoSaveInterval->setValue(dbManager.GetAutoSaveInterval());
    //OnAutoSaveChanged(myAutoSaveEnable->checkState() == Qt::Checked);


    vLayout->addSpacing(12);
    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QProjectDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    setWindowTitle("Project Settings");
}


void QProjectDialog::SetPath(const QString& _path)
{
    myProjectPath->setText(_path);
    QColor color = QDir(_path).exists() ? QColorConstants::Black : QColorConstants::DarkRed;

    QPalette palette = myProjectPath->palette();
    palette.setColor(myProjectPath->foregroundRole(), color);
    myProjectPath->setPalette(palette);
}


void QProjectDialog::OnSelectPath()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select an Unreal Project's Content Folder",
                                                           myProjectPath->text());
    if (folderPath == nullptr)
        return;

    SetPath(folderPath);
}

void QProjectDialog::OnResetPath()
{
    SetPath("");
}


void QProjectDialog::OnPrefixChange()
{
    OnFixChanged(true);
}
void QProjectDialog::OnSuffixChange()
{
    OnFixChanged(false);
}
void QProjectDialog::OnFixChanged(bool _isPrefix)
{
    QLineEdit* lineEdit = _isPrefix ? myPrefixEdit : mySuffixEdit;
    QString txt = lineEdit->text();
    SStringHelper::CleanStringForIdentifier(txt);
    lineEdit->setText(txt);

    UpdateFixResult();
}
void QProjectDialog::UpdateFixResult()
{
    static QString fixResultTemplate = "%1 -> <span style=\" color:%4;\">%2</span>%1<span style=\" color:%5;\">%3</span>";
    myFixResult->setText(fixResultTemplate.arg(ourOriginalAttributeName, myPrefixEdit->text(), mySuffixEdit->text(), ourPrefixColor, ourSuffixColor));
}


void QProjectDialog::OnApplyBtnClicked()
{
    auto& dbManager = DB_Manager::GetDB_Manager();
    if (!dbManager.SetAttributeFixsIfOk(myPrefixEdit->text(), mySuffixEdit->text()))
    {
        qDebug() << "Changing prefix/suffix not ok";

        //const QBrush emptyBrush = QBrush(QColor(255,158,158));
        myPrefixEdit->setStyleSheet("QLineEdit { background: rgb(255,158,158); }");
        mySuffixEdit->setStyleSheet("QLineEdit { background: rgb(255,158,158); }");
        //myPrefixEdit->setBackground(emptyBrush);
        //mySuffixEdit
        return;
    }

    dbManager.SetProjectContentFolderPath(myProjectPath->text());
    dbManager.SetAutoSave(myAutoSaveEnable->checkState() != Qt::Unchecked, myAutoSaveInterval->value());
    QDialog::accept();
}
