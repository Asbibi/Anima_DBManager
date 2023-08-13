#include "qtemplateattributecore.h"

#include "db_manager.h"
#include "qoptionalvalue.h"

QTemplateAttributeCore::QTemplateAttributeCore(TemplateAttribute& _templateAttribute, QWidget* _parent) :
    QTemplateAttributeCore(_templateAttribute.GetSharedParam(), _templateAttribute.GetDefaultAttributeW(), _parent)
{}
QTemplateAttributeCore::QTemplateAttributeCore(AttributeParam& _param, Attribute* _defAttribute, QWidget* _parent) :
    QWidget{_parent},
    myParam{_param},
    myDefAttribute{_defAttribute}
{
    Q_ASSERT(myDefAttribute != nullptr);
    AttributeTypeHelper::Type currentType = myDefAttribute->GetType();

    myFormLayout = new QFormLayout();
    setLayout(myFormLayout);

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
    myTypeComboBox->setCurrentText(AttributeTypeHelper::TypeToString(currentType));
    QObject::connect(myTypeComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttributeCore::OnParamChanged_Type);

    QHBoxLayout* defaultLayout = new QHBoxLayout();
    myDefAttributeEditor = new QAttribute();
    myDefAttributeEditor->UpdateAttribute(myDefAttribute);
    QObject::connect(myDefAttributeEditor, &QAttribute::OnWidgetValueChanged, this, &QTemplateAttributeCore::OnDefaultAttributeEdited);
    myDefAttributeUnavailable = new QLabel("<font color=\"darkred\">Current Configuration is impossible.</font>");
    myDefAttributeUnavailable->setAlignment(Qt::AlignCenter);
    defaultLayout->addWidget(myDefAttributeEditor);
    defaultLayout->addWidget(myDefAttributeUnavailable);
    myFormLayout->addRow("Default:", defaultLayout);

    UpdateLayout(currentType);
    ShowDefaultWidget(true);
}

void QTemplateAttributeCore::PerformTypeSpecificPreparation(AttributeTypeHelper::Type _type)
{
    if (_type == AttributeTypeHelper::Type::Array)
    {
        if (myParam.templateAtt == nullptr)
        {
            myParam.templateAtt = new TemplateAttribute();
        }
    }
    else if (myParam.templateAtt != nullptr)
    {
        delete myParam.templateAtt;
        myParam.templateAtt = nullptr;
        myArrayTemplate = nullptr;
    }
}
void QTemplateAttributeCore::UpdateLayout(AttributeTypeHelper::Type _type)
{
    const int rowToAdd = 1;
    while (myFormLayout->rowCount() > 2)
    {
        myFormLayout->removeRow(rowToAdd);
    }

    PerformTypeSpecificPreparation(_type);

    switch(_type)
    {
        case AttributeTypeHelper::Type::Array:
        {

            Q_ASSERT(myParam.templateAtt != nullptr);
            myArrayTemplate = new QTemplateAttributeCore(*myParam.templateAtt);
            myFormLayout->insertRow(rowToAdd, "Template:", myArrayTemplate);
            QObject::connect(myArrayTemplate, &QTemplateAttributeCore::ParamEdited, this, &QTemplateAttributeCore::OnParamChanged_ArrayTemplate);

#ifdef LIMITS_ON_AARRAYS
            QOptionalValue_Int* minValue = new QOptionalValue_Int();
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            minValue->SetValues(!myParam.ignoreMin, myParam.min_i);
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
            myFormLayout->insertRow(rowToAdd + 1, "Min:", minValue);
            myFormLayout->insertRow(rowToAdd + 2, "Max:", maxValue);
            QObject::connect(minValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMin);
            QObject::connect(minValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MinInt);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxInt);
#endif

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
            QObject::connect(enumComboBox, &QComboBox::currentIndexChanged, this, &QTemplateAttributeCore::OnParamChanged_Enum);
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
            minValue->SetValues(!myParam.ignoreMin, myParam.min_i);
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
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
            structDBComboBox->setCurrentText(myParam.structTable ? myParam.structTable->GetTemplateName() : "---");
            myFormLayout->insertRow(rowToAdd, "Struct DB:", structDBComboBox);
            QObject::connect(structDBComboBox, &QComboBox::currentTextChanged, this, &QTemplateAttributeCore::OnParamChanged_StructDB);
            break;
        }
        case AttributeTypeHelper::Type::ShortString:
        {
            QOptionalValue_Int* maxValue = new QOptionalValue_Int();
            maxValue->SetValues(!myParam.ignoreMax, myParam.max_i);
            maxValue->SetMinimum(1);
            myFormLayout->insertRow(rowToAdd, "Max Lenght:", maxValue);
            QObject::connect(maxValue, &QOptionalValue_Int::OnEnableChanged, this, &QTemplateAttributeCore::OnParamChanged_IgnoreMax);
            QObject::connect(maxValue, &QOptionalValue_Int::OnValueChanged, this, &QTemplateAttributeCore::OnParamChanged_MaxInt);
            break;
        }
        default:
            break;
    }

    if (_type != myDefAttribute->GetType())
    {
        delete myDefAttribute;
        myDefAttribute = AttributeTypeHelper::NewAttributeFromType(_type, myParam);
        myDefAttributeEditor->UpdateAttribute(myDefAttribute);
    }
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
    // TODO: if structure template ...
}


AttributeTypeHelper::Type QTemplateAttributeCore::GetType() const
{
    return AttributeTypeHelper::StringToType(myTypeComboBox->currentText());
}
bool QTemplateAttributeCore::HasConfigValid() const
{
    bool validConfig = AttributeTypeHelper::AreParamValid(GetType(), myParam);
    if (validConfig && myArrayTemplate != nullptr)
    {
        validConfig = myArrayTemplate->HasConfigValid();
    }
    // TODO: if valid && structure template ...
    return validConfig;
}


// ------ Slots ------

void QTemplateAttributeCore::OnParamChanged_Type(const QString& _typeStr)
{
    UpdateLayout(AttributeTypeHelper::StringToType(_typeStr));
    emit ParamEdited(true);
}

void QTemplateAttributeCore::OnParamChanged_IgnoreMin(bool _use)
{
    myParam.ignoreMin = !_use;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_IgnoreMax(int _use)
{
    myParam.ignoreMax = !_use;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MinInt(int _min)
{
    myParam.min_i = _min;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MaxInt(int _max)
{
    myParam.max_i = _max;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MinFloat(float _min)
{
    myParam.min_f = _min;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_MaxFloat(float _max)
{
    myParam.max_f = _max;
    emit ParamEdited();
}
void QTemplateAttributeCore::OnParamChanged_StructDB(const QString& _SDBName)
{
    myParam.structTable = DB_Manager::GetDB_Manager().GetStructureTable(_SDBName);
    emit ParamEdited(true);
}
void QTemplateAttributeCore::OnParamChanged_Enum(int _enumIndex)
{
    myParam.enumeratorIndex = _enumIndex - 1;
    emit ParamEdited(true);
}
void QTemplateAttributeCore::OnParamChanged_ArrayTemplate(bool _withCriticalChange)
{
    emit ParamEdited(_withCriticalChange);
}

void QTemplateAttributeCore::OnDefaultAttributeEdited(const QString& _attributeValueAsText)
{
    myDefAttribute->SetValueFromText(_attributeValueAsText);
    emit ParamEdited(true);
}
