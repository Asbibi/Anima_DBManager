#include "qtemplateattribute.h"


#include <QHBoxLayout>

QTemplateAttribute::QTemplateAttribute(QWidget *parent)
    : QWidget{parent}
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

    ShowDefaultWidget(false);
}

void QTemplateAttribute::UpdateTemplateAttribute(const TemplateAttribute* _attr)
{
    myTemplateCopy = *_attr;

    myApplyBtn->setEnabled(false);
    myApplyBtn->setStyleSheet("");
    myRevertBtn->setEnabled(false);
    myCriticalChanges = false;

    myNameCached = myTemplateCopy.GetName();
    myName->setText(myNameCached);


    myFormLayout->removeWidget(myCoreEditor);
    myCoreEditor = new QTemplateAttributeCore(myTemplateCopy.GetSharedParam(),
                                              myTemplateCopy.GetDefaultAttributeW(),
                                              this);
    myFormLayout->insertRow(1, "", myCoreEditor);

    Q_ASSERT(myCoreEditor->HasConfigValid());


    /*
    myDefAttribute->UpdateAttribute(_attr.GetDefaultAttribute());

    if (_attr.GetType() == AttributeTypeHelper::Type::Array)
    {
        Q_ASSERT(myArrayTemplate != nullptr);
        Q_ASSERT(_attr.GetSharedParam().templateAtt != nullptr);
        myArrayTemplate->ConnectDefaultBtnToAttribute(_attr.GetSharedParam().templateAtt->GetDefaultAttribute());
    }*/
}

void QTemplateAttribute::OnParamEdited(bool withCriticalChange)
{
    myCriticalChanges = myCriticalChanges && withCriticalChange;

    myApplyBtn->setEnabled(true);
    myRevertBtn->setEnabled(true);

    bool configValid = myCoreEditor->HasConfigValid();
    myCoreEditor->ShowDefaultWidget(configValid);

    if (!configValid)
    {
        myApplyBtn->setStyleSheet("QPushButton { color: #DC143C; }");
        myApplyBtn->setEnabled(false);
    }
    else if (myCriticalChanges)
    {
        myApplyBtn->setStyleSheet("QPushButton { color: #DAA520; }");
    }
    else
    {
        myApplyBtn->setStyleSheet("QPushButton { color: black; }");
    }
    ShowDefaultWidget(false);
}

void QTemplateAttribute::ShowDefaultWidget(bool _show)
{
    if (_show)
    {
        myResetAllToDefault->show();
    }
    else
    {
        myResetAllToDefault->hide();
    }
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
    qWarning("Apply Template, may need rework here for AArray:AStructure");
    emit Applied(myNameCached, myCoreEditor->GetType(), myTemplateCopy.GetSharedParam(), myCriticalChanges);
    // TODO : Pass myTemplateCopy directly ? Probably for AArray & AStructure
}
void QTemplateAttribute::OnRevert()
{
    emit Reverted(myNameCached);
}
void QTemplateAttribute::OnApplyDefaultToAll()
{
    emit AppliedDefaultToAll(myNameCached);
}
