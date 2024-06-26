#include "qtemplateattributecore.h"

#include "aarray.h"
#include "astructure.h"
#include "db_manager.h"
#include "qoptionalvalue.h"

QTemplateAttributeCore::QTemplateAttributeCore(TemplateAttribute& _templateAttribute, bool _withNameField, bool _withActive, QWidget* _parent) :
    QWidget{_parent},
    myTemplateAttribute{_templateAttribute}
{
    AttributeTypeHelper::Type currentType = myTemplateAttribute.GetType();

    myFormLayout = new QFormLayout();
    setLayout(myFormLayout);

    if (_withNameField)
    {
        myName = new QLineEdit(_templateAttribute.GetName());
        myFormLayout->addRow("Name:", myName);
        QObject::connect(myName, &QLineEdit::editingFinished, this, &QTemplateAttributeCore::OnParamChanged_Name);
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
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::SkeletalMesh));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::StaticMesh));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Niagara));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Sound));
    myTypeComboBox->addItem(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::Class));
    myTypeComboBox->setCurrentText(AttributeTypeHelper::TypeToString(currentType));
    QObject::connect(myTypeComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttributeCore::OnParamChanged_Type);

    QHBoxLayout* defaultLayout = new QHBoxLayout();
    myDefAttributeEditor = new QAttribute();
    myDefAttributeEditor->UpdateAttribute(myTemplateAttribute.myDefaultAttribute);
    QObject::connect(myDefAttributeEditor, &QAttribute::OnWidgetValueChanged, this, &QTemplateAttributeCore::OnDefaultAttributeEdited);
    myDefAttributeUnavailable = new QLabel("<font color=\"darkred\">Current Configuration is impossible.</font>");
    myDefAttributeUnavailable->setAlignment(Qt::AlignCenter);
    defaultLayout->addWidget(myDefAttributeEditor);
    defaultLayout->addWidget(myDefAttributeUnavailable);
    myFormLayout->addRow("Default:", defaultLayout);

    if (_withActive)
    {
        myActiveCheckBox = new QCheckBox();
        myActiveCheckBox->setCheckState(_templateAttribute.IsActive() ? Qt::Checked : Qt::Unchecked);
        QObject::connect(myActiveCheckBox, &QCheckBox::stateChanged, this, &QTemplateAttributeCore::OnParamChanged_Active);
        myFormLayout->addRow("Active:", myActiveCheckBox);
    }

    UpdateLayout(currentType);
    ShowDefaultWidget(true);
}


void QTemplateAttributeCore::PerformTypeSpecificPreparation(AttributeTypeHelper::Type _type)
{
    if (_type == AttributeTypeHelper::Type::Array
            && myTemplateAttribute.mySharedParam.templateAtt == nullptr)
    {
        myTemplateAttribute.mySharedParam.templateAtt = new TemplateAttribute();
    }

    else if (_type == AttributeTypeHelper::Type::Structure
            && myTemplateAttribute.mySharedParam.templateStruct == nullptr)
    {
        myTemplateAttribute.mySharedParam.templateStruct = new TemplateStructure();
    }
}
void QTemplateAttributeCore::UpdateLayout(AttributeTypeHelper::Type _type)
{
    const AttributeTypeHelper::Type currentType = myTemplateAttribute.GetType();
    const int rowToAdd = myName == nullptr ? 1 : 2;
    const int rowToKeep = rowToAdd + (myActiveCheckBox == nullptr ? 1 : 2);

    if (currentType == AttributeTypeHelper::Type::Array && myArrayTemplate != nullptr)
    {
        myFormLayout->removeRow(myArrayTemplate);
        myArrayTemplate = nullptr;
    }
    else if (currentType == AttributeTypeHelper::Type::Structure && myStructureTemplate != nullptr)
    {
        myFormLayout->removeRow(rowToAdd+2);    // Remove the QPushButton specific to AStructure here since it is placed after myDefAttributeEditor in layout -> if not removed her, myDefAttributeEditor will be deleted instead => crash
        myFormLayout->removeRow(myStructureTemplate);
        myStructureTemplate = nullptr;
    }
    Q_ASSERT(myArrayTemplate == nullptr);
    Q_ASSERT(myStructureTemplate == nullptr);


    PerformTypeSpecificPreparation(_type);

    while (myFormLayout->rowCount() > rowToKeep)
    {
        myFormLayout->removeRow(rowToAdd);
    }


    switch(_type)
    {
        case AttributeTypeHelper::Type::Array:
        {
            Q_ASSERT(myTemplateAttribute.mySharedParam.templateAtt != nullptr);
            myArrayTemplate = new QTemplateAttributeCore(*myTemplateAttribute.mySharedParam.templateAtt, false, false, this);
            myFormLayout->insertRow(rowToAdd, "Template:", myArrayTemplate);
            QObject::connect(myArrayTemplate, &QTemplateAttributeCore::ParamEdited, this, &QTemplateAttributeCore::OnParamChanged_ArrayTemplate);
            QOptionalValue_Int* minValue = new QOptionalValue_Int();
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            minValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMin, myTemplateAttribute.mySharedParam.min_i);
            maxValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMax, myTemplateAttribute.mySharedParam.max_i);
            myFormLayout->insertRow(rowToAdd + 1, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 2, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MinInt);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxInt);
            break;
        }
        case AttributeTypeHelper::Type::Structure:
        {
            Q_ASSERT(myTemplateAttribute.mySharedParam.templateStruct != nullptr);
            myStructureTemplate = new QTemplateStructureCore(*myTemplateAttribute.mySharedParam.templateStruct);
            myFormLayout->insertRow(rowToAdd, "Template:", myStructureTemplate);
            QObject::connect(myStructureTemplate, &QTemplateStructureCore::StructureChanged, this, &QTemplateAttributeCore::OnParamChanged_StructureTemplate);
            QPushButton* resetDefaultValuesBtn = new QPushButton("Reset to Attributes' default values");
            resetDefaultValuesBtn->setMaximumWidth(270);
            myFormLayout->insertRow(rowToAdd+2, "", resetDefaultValuesBtn);
            QObject::connect(resetDefaultValuesBtn, &QPushButton::clicked, this, &QTemplateAttributeCore::OnResetAStructureDefault);
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
            enumComboBox->setCurrentIndex(myTemplateAttribute.mySharedParam.enumeratorIndex + 1);
            myFormLayout->insertRow(rowToAdd, "Enumerator:", enumComboBox);
            QObject::connect(enumComboBox, &QComboBox::currentIndexChanged, this, &QTemplateAttributeCore::OnParamChanged_Enum);
            break;
        }
        case AttributeTypeHelper::Type::Float:
        {
            QOptionalValue_Float* minValue = new QOptionalValue_Float();
            QOptionalValue_Float* maxValue = new QOptionalValue_Float();
            minValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMin, myTemplateAttribute.mySharedParam.min_f);
            maxValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMax, myTemplateAttribute.mySharedParam.max_f);
            myFormLayout->insertRow(rowToAdd, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 1, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Float::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Float::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MinFloat);
            QObject::connect(maxValue, &QOptionalValue_Float::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Float::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxFloat);
            break;
        }
        case AttributeTypeHelper::Type::Int:
        {
            QOptionalValue_Int* minValue = new QOptionalValue_Int();
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            minValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMin, myTemplateAttribute.mySharedParam.min_i);
            maxValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMax, myTemplateAttribute.mySharedParam.max_i);
            myFormLayout->insertRow(rowToAdd, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 1, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MinInt);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxInt);
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
            structDBComboBox->setCurrentText(myTemplateAttribute.mySharedParam.structTable ? myTemplateAttribute.mySharedParam.structTable->GetTemplateName() : "---");
            myFormLayout->insertRow(rowToAdd, "Struct DB:", structDBComboBox);
            QObject::connect(structDBComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttributeCore::OnParamChanged_StructDB);
            break;
        }
        case AttributeTypeHelper::Type::ShortString:
        {
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            maxValue->SetValues(!myTemplateAttribute.mySharedParam.ignoreMax, myTemplateAttribute.mySharedParam.max_i);
            maxValue->SetMinimum(1);
            myFormLayout->insertRow(rowToAdd, "Max Lenght:", maxValue);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxInt);
            break;
        }
        default:
            break;
    }

    if (_type != currentType)
    {
        ReConstructDefaultAttribute(_type);
        AttributeTypeHelper::ResetUselessParamsForType(_type, myTemplateAttribute.mySharedParam);
    }
}
void QTemplateAttributeCore::RefreshDefaultAttributeWidget()
{
    myDefAttributeEditor->UpdateAttribute(myTemplateAttribute.myDefaultAttribute);
}
void QTemplateAttributeCore::ReConstructDefaultAttribute(AttributeTypeHelper::Type _type)
{
    myTemplateAttribute.myDefaultAttribute->PreManualDelete();
    delete myTemplateAttribute.myDefaultAttribute;
    myTemplateAttribute.myDefaultAttribute = AttributeTypeHelper::NewAttributeFromType(_type, myTemplateAttribute);
    RefreshDefaultAttributeWidget();
}

void QTemplateAttributeCore::ShowDefaultWidget(bool _show)
{
    if (_show)
    {
        myDefAttributeEditor->show();
        myDefAttributeUnavailable->hide();
    }
    else
    {
        myDefAttributeEditor->hide();
        myDefAttributeUnavailable->show();
    }

    if (myArrayTemplate != nullptr)
    {
        myArrayTemplate->ShowDefaultWidget(_show);
    }
    if (myStructureTemplate != nullptr)
    {
        myStructureTemplate->ShowDefaultWidget(_show);
    }
}


AttributeTypeHelper::Type QTemplateAttributeCore::GetType() const
{
    return AttributeTypeHelper::StringToType(myTypeComboBox->currentText());
}
bool QTemplateAttributeCore::HasConfigValid() const
{
    bool validConfig = AttributeTypeHelper::AreParamValid(GetType(), myTemplateAttribute.mySharedParam);
    if (validConfig && myArrayTemplate != nullptr)
    {
        validConfig = myArrayTemplate->HasConfigValid();
    }
    if (validConfig && myStructureTemplate != nullptr)
    {
        validConfig = myStructureTemplate->HasConfigValid();
    }
    return validConfig;
}


// ------ Slots ------

void QTemplateAttributeCore::OnParamChanged_Name()
{
    Q_ASSERT(myName != nullptr);
    QString newName = myName->text();
    emit NameChanged(newName);
    myName->setText(newName);
}
void QTemplateAttributeCore::OnParamChanged_Active(int _state)
{
    Q_ASSERT(myActiveCheckBox != nullptr);
    myTemplateAttribute.SetActive(_state == Qt::Checked);
    emit ParamEdited(false);
}
void QTemplateAttributeCore::OnParamChanged_Type(const QString& _typeStr)
{
    AttributeTypeHelper::Type type = AttributeTypeHelper::StringToType(_typeStr);
    UpdateLayout(type);
    emit ParamEdited(true);
    emit TypeChanged(type);
}

void QTemplateAttributeCore::OnParamChanged_IgnoreMin(bool _use)
{
    myTemplateAttribute.mySharedParam.ignoreMin = !_use;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_IgnoreMax(int _use)
{
    myTemplateAttribute.mySharedParam.ignoreMax = !_use;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MinInt(int _min)
{
    myTemplateAttribute.mySharedParam.min_i = _min;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MaxInt(int _max)
{
    myTemplateAttribute.mySharedParam.max_i = _max;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MinFloat(float _min)
{
    myTemplateAttribute.mySharedParam.min_f = _min;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MaxFloat(float _max)
{
    myTemplateAttribute.mySharedParam.max_f = _max;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_StructDB(const QString& _SDBName)
{
    myTemplateAttribute.mySharedParam.structTable = DB_Manager::GetDB_Manager().GetStructureTable(_SDBName);
    RefreshDefaultAttributeWidget();
    emit ParamEdited(true);
}
void QTemplateAttributeCore::OnParamChanged_Enum(int _enumIndex)
{
    myTemplateAttribute.mySharedParam.enumeratorIndex = _enumIndex - 1;
    RefreshDefaultAttributeWidget();
    emit ParamEdited(true);
}
void QTemplateAttributeCore::OnParamChanged_ArrayTemplate(bool _withCriticalChange)
{
    Q_ASSERT(myTemplateAttribute.GetType() == AttributeTypeHelper::Type::Array);
    if (_withCriticalChange)
    {
        AArray* castedDefAttribute = dynamic_cast<AArray*>(myTemplateAttribute.myDefaultAttribute);
        Q_ASSERT(castedDefAttribute != nullptr);
        castedDefAttribute->Empty();
        RefreshDefaultAttributeWidget();
    }

    emit ParamEdited(_withCriticalChange);
}
void QTemplateAttributeCore::OnParamChanged_StructureTemplate(bool _withCriticalChange)
{
    Q_ASSERT(myTemplateAttribute.GetType() == AttributeTypeHelper::Type::Structure);
    if (_withCriticalChange)
    {
        ReConstructDefaultAttribute(AttributeTypeHelper::Type::Structure);
        RefreshDefaultAttributeWidget();
    }
    emit ParamEdited(_withCriticalChange);
}

void QTemplateAttributeCore::OnDefaultAttributeEdited(const QJsonValue& _attributeValue)
{
    myTemplateAttribute.myDefaultAttribute->SetValue_JSON(_attributeValue);
    emit ParamEdited(false);
}
void QTemplateAttributeCore::OnResetAStructureDefault()
{
    Q_ASSERT(myTemplateAttribute.GetType() == AttributeTypeHelper::Type::Structure);
    AStructure* defaultAsAStructure = dynamic_cast<AStructure*>(myTemplateAttribute.myDefaultAttribute);
    defaultAsAStructure->ResetValueToDefaults();
    RefreshDefaultAttributeWidget();
    emit ParamEdited(false);
}
