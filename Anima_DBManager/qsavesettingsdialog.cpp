#include "qsavesettingsdialog.h"

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



QSaveSettingsDialog::QSaveSettingsDialog(QWidget* _parent) :
    QDialog(_parent)
{
    auto& dbManager = DB_Manager::GetDB_Manager();

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    const QString titleStyle = "font-weight: bold";

    // Auto-Save
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

    // Compression
    auto* compressSaveTitle = new QLabel("Save Compression");
    compressSaveTitle->setStyleSheet(titleStyle);
    vLayout->addWidget(compressSaveTitle);
    vLayout->addSpacing(3);
    QFormLayout* saveCompressionLayout = new QFormLayout();
    myCompressSaveFile = new QCheckBox();
    saveCompressionLayout->addRow("Compress Save file:", myCompressSaveFile);
    myCompressSaveFile->setChecked(SaveManager::IsCurrentSaveFileWithCompression());
    vLayout->addLayout(saveCompressionLayout);


    vLayout->addSpacing(12);
    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QSaveSettingsDialog::OnApplyBtnClicked);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    setWindowTitle("Save Settings");
}


void QSaveSettingsDialog::OnApplyBtnClicked()
{
    DB_Manager::GetDB_Manager().SetAutoSave(myAutoSaveEnable->checkState() != Qt::Unchecked, myAutoSaveInterval->value());
    SaveManager::SetSaveWithCompressionChoice(myCompressSaveFile->checkState() != Qt::Unchecked);
    QDialog::accept();
}
