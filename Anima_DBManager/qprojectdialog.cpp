#include "qprojectdialog.h"

#include "db_manager.h"
#include "savemanager.h"
#include "sstringhelper.h"
#include "unrealprojecthelper.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>


const QString QProjectDialog::ourOriginalAttributeName = "AttributeName";
const QString QProjectDialog::ourPrefixColor = QColorConstants::DarkBlue.name();
const QString QProjectDialog::ourSuffixColor = QColorConstants::DarkGreen.name();
const QString QProjectDialog::ourContentColor = QColorConstants::DarkYellow.name();


QProjectDialog::QProjectDialog(QWidget* _parent) :
    QDialog(_parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const QString titleStyle = "font-weight: bold";

    // Project Path
    auto* pathTitle = new QLabel("Unreal Project Path :");
    pathTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(pathTitle);
    vLayout->addSpacing(3);
    const auto& dbManager = DB_Manager::GetDB_Manager();
    myProjectPath = new QLabel();
    myRelativeProjectPath = new QLabel();
    vLayout->addWidget(myProjectPath);
    auto* relativeLabel = new QLabel("Save as relative path to the save file :");
    QHBoxLayout* relativePathLayout = new QHBoxLayout();
    relativePathLayout->addWidget(relativeLabel);
    myProjectPathIsRelative = new QCheckBox();
    relativePathLayout->addWidget(myProjectPathIsRelative);
    vLayout->addLayout(relativePathLayout);
    vLayout->addWidget(myRelativeProjectPath);
    vLayout->addSpacing(3);
    QPushButton* changeBtn = new QPushButton("Select Directory");
    QObject::connect(changeBtn, &QPushButton::clicked, this, &QProjectDialog::OnSelectPath);
    QPushButton* resetBtn = new QPushButton("Clear");
    QObject::connect(resetBtn, &QPushButton::clicked, this, &QProjectDialog::OnResetPath);
    QHBoxLayout* pathBtnLayout = new QHBoxLayout();
    pathBtnLayout->addWidget(changeBtn);
    pathBtnLayout->addWidget(resetBtn);
    vLayout->addLayout(pathBtnLayout);

    InitPath(dbManager.GetRawProjectContentFolderPath());   // Init after all widget have been created
    QObject::connect(myProjectPathIsRelative, &QCheckBox::toggled, this, &QProjectDialog::OnRelativePathToggle);  // Connect after init

    vLayout->addSpacing(6);
    auto* uassetTitle = new QLabel("Asset Attribute file filters:");
    uassetTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(uassetTitle);
    vLayout->addSpacing(3);
    const int assetTypeCount = AttributeTypeHelper::assetTypes.count();
    myUAssetRegex = new QTableWidget();
    auto* hHeaders = myUAssetRegex->horizontalHeader();
    hHeaders->hide();
    hHeaders->setStretchLastSection(true);
    myUAssetRegex->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    myUAssetRegex->setColumnCount(1);
    myUAssetRegex->setRowCount(assetTypeCount);
    QStringList vLabels{};
    for (int i = 0; i < assetTypeCount; i++)
    {
        vLabels.push_back(AttributeTypeHelper::TypeToString(AttributeTypeHelper::assetTypes[i]));
        myUAssetRegex->setItem(i, 0, new QTableWidgetItem(dbManager.GetAAssetRegex(AttributeTypeHelper::assetTypes[i])));
    }
    myUAssetRegex->setVerticalHeaderLabels(vLabels);
    myUAssetRegex->setMaximumWidth(200);
    vLayout->addWidget(myUAssetRegex);
    //myUAssetRegex->adjustSize();

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

    vLayout->addSpacing(6);
    auto* compressSaveTitle = new QLabel("Save Compression");
    compressSaveTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(compressSaveTitle);
    vLayout->addSpacing(3);
    QFormLayout* saveCompressionLayout = new QFormLayout();
    myCompressSaveFile = new QCheckBox();
    saveCompressionLayout->addRow("Compress Save file:", myCompressSaveFile);
    myCompressSaveFile->setChecked(SaveManager::IsCurrentSaveFileWithCompression());
    QObject::connect(myCompressSaveFile, &QCheckBox::checkStateChanged, this, &QProjectDialog::OnSaveCompressionChecked);
    vLayout->addLayout(saveCompressionLayout);


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


void QProjectDialog::InitPath(const QString& _path)
{
    mySaveFilePath = SaveManager::GetCurrentSaveFile();
    if (mySaveFilePath.isEmpty())
    {
        myProjectPathIsRelative->setEnabled(false);
    }

    if (_path.isEmpty() || QFileInfo(_path).isAbsolute())
    {
        SetPath(_path);
        return;
    }

    myProjectPathIsRelative->setChecked(true);
    SetPath(UnrealProjectHelper::GetAbsolutePathFromRelative(mySaveFilePath, _path));
}
void QProjectDialog::SetPath(const QString& _path)
{
    myProjectPathText = _path;
    bool isRelative = myProjectPathIsRelative->isChecked();

    static QString projectPathFinalTemplate = "%1<span style=\" color:%2;\">/Content/</span>";
    myProjectPath->setText(_path.isEmpty() ? "- No Project Folder -" : projectPathFinalTemplate.arg(_path, ourContentColor));
    myRelativeProjectPath->setText(isRelative && !_path.isEmpty() ? projectPathFinalTemplate.arg(UnrealProjectHelper::GetRelativePathFromAbsolute(mySaveFilePath, _path), ourContentColor): "");
    myRelativeProjectPath->setVisible(isRelative);

    QColor color = UnrealProjectHelper::IsPathValidUnrealProject(_path) ? QColorConstants::Black : QColorConstants::DarkRed;

    QPalette palette = myProjectPath->palette();
    palette.setColor(myProjectPath->foregroundRole(), color);
    myProjectPath->setPalette(palette);
    myRelativeProjectPath->setPalette(palette);
}


void QProjectDialog::OnRelativePathToggle()
{
    SetPath(myProjectPathText);
}
void QProjectDialog::OnSelectPath()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select an Unreal Project File",
        QDir::homePath(),
        "Unreal Project Files (*.uproject)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    SetPath(QFileInfo(fileName).path());
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


void QProjectDialog::OnSaveCompressionChecked(Qt::CheckState _checkState)
{
    SaveManager::SetSaveWithCompressionChoice(_checkState != Qt::Unchecked);
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

    const int assetTypeCount = AttributeTypeHelper::assetTypes.count();
    for (int i = 0; i < assetTypeCount; i++)
    {
        dbManager.SetAAssetRegex(AttributeTypeHelper::assetTypes[i], myUAssetRegex->item(i, 0)->text());
    }

    if (myProjectPathIsRelative->isChecked())
    {
        dbManager.SetProjectContentFolderPath(UnrealProjectHelper::GetRelativePathFromAbsolute(mySaveFilePath, myProjectPathText));
    }
    else
    {
        dbManager.SetProjectContentFolderPath(myProjectPathText);
    }
    dbManager.SetAutoSave(myAutoSaveEnable->checkState() != Qt::Unchecked, myAutoSaveInterval->value());
    QDialog::accept();
}
