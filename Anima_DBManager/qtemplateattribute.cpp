#include "qtemplateattribute.h"

#include "db_manager.h"
#include "qoptionalvalue.h"

#include <QHBoxLayout>

QTemplateAttribute::QTemplateAttribute(bool _withNameField, QWidget *parent)
    : QWidget{parent}
{
    myFormLayout = new QFormLayout();
    setLayout(myFormLayout);

    myName = new QLineEdit();
    if (_withNameField)
    {
        myFormLayout->addRow("Name:", myName);
        QObject::connect(myName, &QLineEdit::editingFinished, this, &QTemplateAttribute::OnNameEdited);
    }
    else
    {
        myName->hide();
    }

    myTypeComboBox = new QComboBox();
    myFormLayout->addRow("Type:", myTypeComboBox);

    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Bool));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Enum));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Float));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Int));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::ShortString));

    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::TableString));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Reference));

    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Array));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Structure));

    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::UAsset));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Texture));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Mesh));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Niagara));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Sound));


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

    QHBoxLayout* defaultLayout = new QHBoxLayout();
    myDefAttribute = new QAttribute();
    myResetAllToDefault = new QPushButton("Reset All To Default");
    myResetAllToDefault->setStyleSheet("QPushButton { color: #DAA520; }");    
    QObject::connect(myResetAllToDefault, &QPushButton::clicked, this, &QTemplateAttribute::OnApplyDefaultToAll);
    myDefAttributeUnavailable = new QLabel("<font color=\"darkred\">Apply or Revert to edit the default value.</font>");
    myDefAttributeUnavailable->setAlignment(Qt::AlignCenter);
    defaultLayout->addWidget(myDefAttribute);
    defaultLayout->addWidget(myResetAllToDefault);
    defaultLayout->addWidget(myDefAttributeUnavailable);
    defaultLayout->setContentsMargins(0,0,0,0);
    myFormLayout->addRow("Default:", defaultLayout);
    ShowDefaultWidgets(false);
}

void QTemplateAttribute::UpdateContent(const TemplateAttribute& _attr)
{
    myParam = _attr.GetSharedParam();
    myApplyBtn->setEnabled(false);
    myApplyBtn->setStyleSheet("");
    myRevertBtn->setEnabled(false);
    myParamHasCriticalChanged = false;

    myNameCached = _attr.GetName();
    myName->setText(myNameCached);
    UpdateContent(_attr.GetType());

    if (_attr.HasValidSharedParam())
    {
        ShowDefaultWidgets(true);
    }
    else
    {
        ShowDefaultWidgets(false);
    }
    myDefAttribute->UpdateAttribute(_attr.GetDefaultAttribute());

    if (_attr.GetType() == AttributeTypeHelper::Type::Array)
    {
        Q_ASSERT(myArrayTemplate != nullptr);
        Q_ASSERT(_attr.GetSharedParam().templateAtt != nullptr);
        myArrayTemplate->ConnectDefaultBtnToAttribute(_attr.GetSharedParam().templateAtt->GetDefaultAttribute());
    }
}


void QTemplateAttribute::UpdateContent(AttributeTypeHelper::Type _type)
{
    const int rowToAdd = myName->isHidden() ? 1 : 2;
    while (myFormLayout->rowCount() > 2 + rowToAdd)
    {
        myFormLayout->removeRow(rowToAdd);
    }

    QObject::disconnect(myTypeComboBox, nullptr, this, nullptr);
    myTypeComboBox->setCurrentText(AttributeTypeHelper::TypeToString(_type));
    QObject::connect(myTypeComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttribute::OnParamChanged_Type);

    if (_type != AttributeTypeHelper::Type::Array)
    {
        if ( myParam.templateAtt != nullptr)
        {
            delete myParam.templateAtt;
            myParam.templateAtt = nullptr;
        }
        if (myArrayTemplate != nullptr)
        {
            myFormLayout->removeRow(myArrayTemplate);
            myArrayTemplate = nullptr;
        }
    }
    else if (myParam.templateAtt == nullptr)    // implicitly : && _type == AttributeTypeHelper::Type::Array
    {
        myParam.templateAtt = new TemplateAttribute();
    }

    switch(_type)
    {
        case AttributeTypeHelper::Type::Array:
        {
            Q_ASSERT(myParam.templateAtt != nullptr);
            myArrayTemplate = new QTemplateAttribute(false);
            myFormLayout->insertRow(rowToAdd, "Template:", myArrayTemplate);
            myArrayTemplate->UpdateContent(*myParam.templateAtt);
            QObject::connect(myArrayTemplate, &QTemplateAttribute::Applied, this, &QTemplateAttribute::OnParamChanged_Template);
            QObject::connect(myArrayTemplate, &QTemplateAttribute::Reverted, this, &QTemplateAttribute::OnParamReverted_Template);
            QObject::connect(myArrayTemplate, &QTemplateAttribute::AppliedDefaultToAll, this, &QTemplateAttribute::OnParamAppliedDef_Template);

            /*
            QOptionalValue_Int* minValue = new QOptionalValue_Int();
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            minValue->SetValues(!myParam.ignoreMin, myParam.min_i);
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
            myFormLayout->insertRow(rowToAdd + 1, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 2, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MinInt);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MaxInt);
            */
            break;
        }
        case AttributeTypeHelper::Type::Enum:
        {            
            QComboBox* enumComboBox = new QComboBox();
            const DB_Manager& DB = DB_Manager::GetDB_Manager();
            const int enumCount = DB.GetEnumCount();
            enumComboBox->addItem("---");
            for (int i = 0; i < enumCount; i++)
            {
                enumComboBox->addItem(DB.GetEnum(i)->GetName());
            }
            enumComboBox->setCurrentIndex(myParam.enumeratorIndex + 1);
            myFormLayout->insertRow(rowToAdd, "Enumerator:", enumComboBox);
            QObject::connect(enumComboBox, &QComboBox::currentIndexChanged, this, &QTemplateAttribute::OnParamChanged_Enum);
            break;
        }
        case AttributeTypeHelper::Type::Float:
        {
            QOptionalValue_Float* minValue = new QOptionalValue_Float();
            QOptionalValue_Float* maxValue = new QOptionalValue_Float();
            minValue->SetValues(!myParam.ignoreMin, myParam.min_f);
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_f);
            myFormLayout->insertRow(rowToAdd, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 1, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Float::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Float::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MinFloat);
            QObject::connect(maxValue, &QOptionalValue_Float::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Float::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MaxFloat);
            break;
        }
        case AttributeTypeHelper::Type::Int:
        {
            QOptionalValue_Int* minValue = new QOptionalValue_Int();
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            minValue->SetValues(!myParam.ignoreMin, myParam.min_i);
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
            myFormLayout->insertRow(rowToAdd, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 1, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MinInt);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MaxInt);
            break;
        }
        case AttributeTypeHelper::Type::Reference:
        {
            QComboBox* structDBComboBox = new QComboBox();
            const DB_Manager& DB = DB_Manager::GetDB_Manager();
            const int structCount = DB.GetStructuresCount();
            structDBComboBox->addItem("---");
            for (int i = 0; i < structCount; i++)
            {
                structDBComboBox->addItem(DB.GetStructureTable(i)->GetTemplateName());
            }
            structDBComboBox->setCurrentText(myParam.structTable ? myParam.structTable->GetTemplateName() : "---");
            myFormLayout->insertRow(rowToAdd, "Struct DB:", structDBComboBox);
            QObject::connect(structDBComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttribute::OnParamChanged_StructDB);
            break;
        }
        case AttributeTypeHelper::Type::ShortString:
        {
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
            maxValue->SetMinimum(1);
            myFormLayout->insertRow(rowToAdd, "Max Lenght:", maxValue);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttribute::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttribute::OnParamChanged_MaxInt);
            break;
        }
        default:
            break;
    }
}

void QTemplateAttribute::OnParamEdited()
{
    myApplyBtn->setEnabled(true);
    myRevertBtn->setEnabled(true);

    if (!AttributeTypeHelper::AreParamValid(AttributeTypeHelper::StringToType(myTypeComboBox->currentText()), myParam))
    {
        myApplyBtn->setStyleSheet("QPushButton { color: #DC143C; }");
        myApplyBtn->setEnabled(false);
    }
    else if (myParamHasCriticalChanged)
    {
        myApplyBtn->setStyleSheet("QPushButton { color: #DAA520; }");
    }
    else
    {
        myApplyBtn->setStyleSheet("QPushButton { color: black; }");
    }
    ShowDefaultWidgets(false);
}
void QTemplateAttribute::ShowDefaultWidgets(bool _show)
{
    if (_show)
    {
        myDefAttribute->show();
        myResetAllToDefault->show();
        myDefAttributeUnavailable->hide();
    }
    else
    {
        myDefAttribute->hide();
        myResetAllToDefault->hide();
        myDefAttributeUnavailable->show();
    }
}



// Slots

void QTemplateAttribute::OnParamChanged_Type(const QString& _typeStr)
{
    myParamHasCriticalChanged = true;
    UpdateContent(AttributeTypeHelper::StringToType(_typeStr));
    OnParamEdited();
}

void QTemplateAttribute::OnParamChanged_IgnoreMin(bool _use)
{
    myParam.ignoreMin = !_use;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_IgnoreMax(int _use)
{
    myParam.ignoreMax = !_use;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_MinInt(int _min)
{
    myParam.min_i = _min;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_MaxInt(int _max)
{
    myParam.max_i = _max;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_MinFloat(float _min)
{
    myParam.min_f = _min;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_MaxFloat(float _max)
{
    myParam.max_f = _max;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_StructDB(const QString& _SDBName)
{
    myParam.structTable = DB_Manager::GetDB_Manager().GetStructureTable(_SDBName);
    myParamHasCriticalChanged = true;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_Enum(int _enumIndex)
{
    myParam.enumeratorIndex = _enumIndex - 1;
    myParamHasCriticalChanged = true;
    OnParamEdited();
}
void QTemplateAttribute::OnParamChanged_Template(const QString& _name, AttributeTypeHelper::Type _newType, const AttributeParam& _param)
{
    myDefAttribute->EmptyArrayAttribute();
    myParam.templateAtt->SetNewValues(_newType, _param);
    myParamHasCriticalChanged = true;
    OnParamEdited();
}
void QTemplateAttribute::OnParamReverted_Template()
{
    Q_ASSERT(myArrayTemplate != nullptr);
    myArrayTemplate->UpdateContent(*myParam.templateAtt);
}
void QTemplateAttribute::OnParamAppliedDef_Template()
{
    qDebug() << "TODO";
}
void QTemplateAttribute::ConnectDefaultBtnToAttribute(const Attribute* _attribute)
{
    QObject::connect(myDefAttribute, &QAttribute::OnWidgetValueChanged, _attribute, &Attribute::SetValueFromText);
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
    emit Applied(myNameCached, AttributeTypeHelper::StringToType(myTypeComboBox->currentText()), myParam, myParamHasCriticalChanged);
}
void QTemplateAttribute::OnRevert()
{
    emit Reverted(myNameCached);
}
void QTemplateAttribute::OnApplyDefaultToAll()
{
    emit AppliedDefaultToAll(myNameCached);
}
