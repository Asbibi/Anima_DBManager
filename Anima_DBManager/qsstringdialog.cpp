#include "qsstringdialog.h"

#include "db_manager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>

QSStringDialog::QSStringDialog(QString& _outTableId, QString& _outStringId, QWidget* parent) :
    QDialog(parent),
    myTableName(_outTableId),
    myStringIdentifier(_outStringId)
{
    // ----------
    setWindowTitle("Select a String");
    setModal(true);

    // ----------
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    QGroupBox* comboWidget = new QGroupBox("Identifiers");
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    comboWidget->setLayout(hLayout);
    myTableComboBox = new QComboBox();
    myTableComboBox->setEditable(true);
    myStringComboBox = new QComboBox();
    myStringComboBox->setEditable(true);
    hLayout->addWidget(myTableComboBox);
    hLayout->addWidget(myStringComboBox);

    vLayout->addWidget(comboWidget);


    QWidget* previewWidget = new QWidget();
    QHBoxLayout* hLayoutPreview = new QHBoxLayout(this);
    previewWidget->setLayout(hLayoutPreview);
    auto* scrollArea = new QScrollArea();
    myStringPreview = new QLabel(scrollArea);
    scrollArea->setWidget(myStringPreview);
    scrollArea->setMinimumWidth(300);
    hLayoutPreview->addWidget(scrollArea);
    auto* languageComboBox = new QComboBox();
    hLayoutPreview->addWidget(languageComboBox);
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        languageComboBox->addItem(SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
    }
    QObject::connect(languageComboBox, &QComboBox::currentIndexChanged, this, &QSStringDialog::SetLanguage);
    vLayout->addWidget(previewWidget);


    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout(this);
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QDialog::accept);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    // ----------
    const auto& DB = DB_Manager::GetDB_Manager();
    const int strTableCount = DB.GetStringTableCount();
    for (int i = 0; i < strTableCount; i++)
    {
        myTableComboBox->addItem(DB.GetStringTable(i)->GetTableName());
    }
    myTableComboBox->setCurrentText(myTableName);
    UpdateStringComboBox();
    UpdateStringPreview();
    myStringComboBox->setCurrentText(myStringIdentifier);
    QObject::connect(myTableComboBox, &QComboBox::currentTextChanged, this, &QSStringDialog::SetTableName);
}


void QSStringDialog::UpdateStringComboBox()
{
    QObject::disconnect(myStringComboBox, nullptr, nullptr, nullptr);
    while (myStringComboBox->count() > 0)
    {
        myStringComboBox->removeItem(0);
    }

    const auto& DB = DB_Manager::GetDB_Manager();
    const auto* table = DB.GetStringTable(myTableName);
    if (!table)
    {
        return;
    }

    const int stringCountInTable = table->GetStringItemCount();
    for (int i = 0; i<stringCountInTable; i++)
    {
        myStringComboBox->addItem(table->GetStringItem(i)->GetIdentifier());
    }
    QObject::connect(myStringComboBox, &QComboBox::currentTextChanged, this, &QSStringDialog::SetStringName);
}

void QSStringDialog::UpdateStringPreview()
{
    const auto& DB = DB_Manager::GetDB_Manager();
    if (!DB.AreValidIdentifiers(myTableName, myStringIdentifier))
    {
        myStringPreview->setText("INVALID");
    }
    else
    {
        const auto* stringItem = DB.GetStringTable(myTableName)->GetStringItem(myStringIdentifier);
        myStringPreview->setText(stringItem->GetString(myLanguage));
    }
    myStringPreview->resize(myStringPreview->sizeHint());
}


void QSStringDialog::SetTableName(const QString &text)
{
    myTableName = text;
    UpdateStringComboBox();
    myStringIdentifier = myStringComboBox->currentText();
    UpdateStringPreview();
}

void QSStringDialog::SetStringName(const QString &text)
{
    myStringIdentifier = text;
    UpdateStringPreview();
}

void QSStringDialog::SetLanguage(const int languageIndex)
{
    myLanguage = (SStringHelper::SStringLanguages)languageIndex;
    UpdateStringPreview();
}
