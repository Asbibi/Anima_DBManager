#include "qtemplateattribute.h"


#include <QHBoxLayout>

QTemplateAttribute::QTemplateAttribute(QWidget *parent)
    : QWidget{parent}, myTemplateCopy{nullptr}, myCoreEditor{nullptr}
{
    myFormLayout = new QFormLayout();
    setLayout(myFormLayout);

    myName = new QLineEdit();
    myFormLayout->addRow("Name:", myName);
    QObject::connect(myName, &QLineEdit::editingFinished, this, &QTemplateAttribute::OnNameEdited);


    QHBoxLayout* btnLayout = new QHBoxLayout();
    myApplyBtn = new QPushButton("Apply");
    myRevertBtn = new QPushButton("Revert");
    myApplyBtn->setMaximumWidth(50);
    myRevertBtn->setMaximumWidth(50);
    QObject::connect(myApplyBtn, &QPushButton::clicked, this, &QTemplateAttribute::OnApply);
    QObject::connect(myRevertBtn, &QPushButton::clicked, this, &QTemplateAttribute::OnRevert);
    btnLayout->addWidget(myApplyBtn);
    btnLayout->addWidget(myRevertBtn);
    myFormLayout->addRow(btnLayout);


    myResetAllToDefault = new QPushButton("Reset All Attributes To Default");
    myResetAllToDefault->setStyleSheet("QPushButton { color: #DAA520; }");
    QObject::connect(myResetAllToDefault, &QPushButton::clicked, this, &QTemplateAttribute::OnApplyDefaultToAll);
    myFormLayout->addRow("Default:", myResetAllToDefault);

    ShowDefaultWidget(true);
}
QTemplateAttribute::~QTemplateAttribute()
{
    DeleteTemplateIfExists();
}
void QTemplateAttribute::DeleteTemplateIfExists()
{
    if (myTemplateCopy == nullptr)
        return;

    delete myTemplateCopy;
    myTemplateCopy = nullptr;
}

void QTemplateAttribute::UpdateTemplateAttribute(const TemplateAttribute* _attr)
{
    DeleteTemplateIfExists();
    myTemplateCopy = new TemplateAttribute(*_attr);

    myApplyBtn->setEnabled(false);
    myApplyBtn->setStyleSheet("");
    myRevertBtn->setEnabled(false);
    myCriticalChanges = false;

    myNameCached = myTemplateCopy->GetName();
    myName->setText(myNameCached);


    if (myCoreEditor != nullptr) {
        myCoreEditor->disconnect();
        myFormLayout->removeRow(1);
    }
    myCoreEditor = new QTemplateAttributeCore(*myTemplateCopy, this);
    myFormLayout->insertRow(1, "", myCoreEditor);
    QObject::connect(myCoreEditor, &QTemplateAttributeCore::ParamEdited, this, &QTemplateAttribute::OnParamEdited);

    Q_ASSERT(myCoreEditor->HasConfigValid());
}

void QTemplateAttribute::OnParamEdited(bool withCriticalChange)
{
    const bool oldCritical = myCriticalChanges;
    myCriticalChanges = myCriticalChanges || withCriticalChange;

    myApplyBtn->setEnabled(true);
    myRevertBtn->setEnabled(true);

    const bool oldInvalid = myInvalidChanges;
    myInvalidChanges = !myCoreEditor->HasConfigValid();
    myCoreEditor->ShowDefaultWidget(!myInvalidChanges);

    if (myInvalidChanges)
    {
        myApplyBtn->setStyleSheet("QPushButton { color: #DC143C; }");
        myApplyBtn->setEnabled(false);
    }
    else if (oldCritical != myCriticalChanges || oldInvalid){
        myApplyBtn->setStyleSheet(myCriticalChanges ?
            "QPushButton { color: #DAA520; }" :
            "QPushButton { color: black; }");
        ShowDefaultWidget(false);
    }
}

void QTemplateAttribute::ShowDefaultWidget(bool _show)
{
    myResetAllToDefault->setEnabled(_show);
}






void QTemplateAttribute::OnNameEdited()
{
    QString _newName = myName->text();
    _newName.replace(' ','_');
    emit NameChanged(myNameCached, _newName);
    myName->setText(_newName);
    myNameCached = _newName;
}
void QTemplateAttribute::OnApply()
{
    Q_ASSERT(myTemplateCopy != nullptr);
    emit Applied(myNameCached, *myTemplateCopy, myCriticalChanges);
    ShowDefaultWidget(true);
}
void QTemplateAttribute::OnRevert()
{
    emit Reverted(myNameCached);
    ShowDefaultWidget(true);
}
void QTemplateAttribute::OnApplyDefaultToAll()
{
    emit AppliedDefaultToAll(myNameCached);
}
