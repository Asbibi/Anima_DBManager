#include "qprojectdialog.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "db_manager.h"


QProjectDialog::QProjectDialog(QWidget* _parent) :
    QDialog(_parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    // Project Path
    auto* pathTitle = new QLabel("Unreal Project Path :");
    pathTitle->setStyleSheet("font-weight: bold");
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
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Struct DataTable Directory",
                                                           myProjectPath->text());
    if (folderPath == nullptr)
        return;

    SetPath(folderPath);
}

void QProjectDialog::OnResetPath()
{
    SetPath("");
}

void QProjectDialog::OnApplyBtnClicked()
{
    DB_Manager::GetDB_Manager().SetProjectContentFolderPath(myProjectPath->text());
    QDialog::accept();
}
