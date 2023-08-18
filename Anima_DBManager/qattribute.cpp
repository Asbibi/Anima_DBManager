#include "qattribute.h"

#include "aarray.h"
#include "aasset.h"
#include "aamesh.h"
#include "aaniagara.h"
#include "aasound.h"
#include "aatexture.h"
#include "abool.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "atablestring.h"
#include "areference.h"
#include "astructure.h"
#include "qsstring.h"
#include "qarraylabel.h"
#include "qastructurelabel.h"
#include "qassetlabel.h"
#include "qassettexture.h"
#include "qreflabel.h"

#include <QDebug>
#include <QtGlobal>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QSpinBox>



QAttribute::QAttribute(QWidget* parent) :
    QWidget(parent)
{
    auto* myLayout = new QHBoxLayout();
    myLayout->setAlignment(Qt::AlignCenter);
    myLayout->setContentsMargins(0,0,0,0);
    setLayout(myLayout);
}

QAttribute::~QAttribute()
{
    DeleteContent();
}

void QAttribute::LogErrorCast() const
{
    qCritical() << "-- CAST FAILED -- Attribute or Content Cast failed with type " << (int)myType;
}

void QAttribute::DeleteContent()
{
    if (myEditButton != nullptr)
    {
        delete myEditButton;
        myEditButton = nullptr;
    }

    if (myContent != nullptr)
    {
        delete myContent;
        myContent = nullptr;
    }
}

void QAttribute::RebuildAttributeWidget(const Attribute* _attribute)
{
    // We assume that myAttributePtr != _attribute, so need to reconnect to the right attribute object
    QObject::disconnect(this, nullptr, nullptr, nullptr);
    QObject::connect(_attribute, &Attribute::OnValueChanged, this, &QAttribute::UpdateAttribute);
    QObject::connect(this, &QAttribute::OnWidgetValueChanged, _attribute, &Attribute::SetValueFromText);

    const AttributeTypeHelper::Type _newType = _attribute ? _attribute->GetType() : AttributeTypeHelper::Type::Invalid;
    if (_newType != myType)
    {
        RebuildWidgetFromType(_newType);
    }

    myAttributePtr = _attribute;
    myType = _newType;
}

void QAttribute::RebuildWidgetFromType(const AttributeTypeHelper::Type _type)
{
    DeleteContent();

    switch(_type)
    {
        case AttributeTypeHelper::Type::Array :
        {
            QArrayLabel* content = new QArrayLabel(this);
            QObject::connect(content, &QArrayLabel::OnValueEdited,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;

            BuildMoreButton();
            QObject::connect(myEditButton, &QPushButton::clicked,
                                 content, &QArrayLabel::EditValue);
            break;
        }
        case AttributeTypeHelper::Type::Bool :
        {
            QCheckBox* content = new QCheckBox(this);
            QObject::connect(content, &QCheckBox::clicked,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            layout()->addWidget(myContent);
            break;
        }
        case AttributeTypeHelper::Type::Enum :
        {
            QComboBox* content = new QComboBox(this);
            // Connection Handled in Update since it needs to be temporary disabled
            myContent = content;
            break;
        }
        case AttributeTypeHelper::Type::Float :
        {
            QDoubleSpinBox* content = new QDoubleSpinBox(this);
            QObject::connect(content, &QDoubleSpinBox::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case AttributeTypeHelper::Type::Int :
        {
            QSpinBox* content = new QSpinBox(this);
            QObject::connect(content, &QSpinBox::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case AttributeTypeHelper::Type::ShortString :
        {
            QLineEdit* content = new QLineEdit(this);
            QObject::connect(content, &QLineEdit::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case AttributeTypeHelper::Type::TableString :
        {
            QSString* content = new QSString(this);
            QObject::connect(content, &QSString::OnValueEdited,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;

            BuildMoreButton();
            QObject::connect(myEditButton, &QPushButton::clicked,
                                 content, &QSString::EditValue);
            break;
        }
        case AttributeTypeHelper::Type::Structure :
        {
            QAStructureLabel* content = new QAStructureLabel(this);
            QObject::connect(content, &QAStructureLabel::OnValueEdited,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;

            BuildMoreButton();
            QObject::connect(myEditButton, &QPushButton::clicked,
                                 content, &QAStructureLabel::EditValue);
            break;
        }
        case AttributeTypeHelper::Type::Texture :
        {
            QAssetTexture* content = new QAssetTexture(this);
            QObject::connect(content, &QAssetTexture::OnValueEdited,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;

            BuildMoreButton();
            QObject::connect(myEditButton, &QPushButton::clicked,
                                 content, &QAssetTexture::OpenFileDialog);
            break;
        }
#define CASE_ATTRIBUTE(type, typeStr, typeFile, expr, ext) \
case AttributeTypeHelper::Type::type : \
{ \
    QAssetLabel* content = new QAssetLabel(AttributeTypeHelper::Type::type, typeStr, typeFile" ("#expr"." + ext + ")", this); \
    QObject::connect(content, &QAssetLabel::OnValueEdited, \
                         this, &QAttribute::ContentStateChanged); \
    myContent = content;    \
\
    BuildMoreButton(); \
    QObject::connect(myEditButton, &QPushButton::clicked, \
                         content, &QAssetLabel::OpenFileDialog); \
    break; \
}

        CASE_ATTRIBUTE(UAsset, "UAsset", "Ureal Assets", *, AAsset::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(Mesh, "Mesh", "3D Files", *, AATexture::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(Niagara, "Niagara", "Niagara Systems", P_*, AAsset::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(Sound, "Sound", "Sounds", *, AASound::GetStaticAssetFileExtension());
#undef CASE_ATTRIBUTE
        case AttributeTypeHelper::Type::Reference :
        {
            QRefLabel* content = new QRefLabel(this);
            QObject::connect(content, &QRefLabel::OnValueEdited,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;

            BuildMoreButton();
            QObject::connect(myEditButton, &QPushButton::clicked,
                                 content, &QRefLabel::EditValue);
            break;
        }
        default:
            break;
    }



    if (myContent != nullptr)
    {
        layout()->addWidget(myContent);
    }
    if (myEditButton != nullptr)
    {
        layout()->addItem(new QSpacerItem(10,5));
        layout()->addWidget(myEditButton);
    }
}

void QAttribute::BuildMoreButton()
{
    myEditButton = new QPushButton("+", this);
    myEditButton->setMaximumWidth(20);
}

void QAttribute::UpdateAttribute(const Attribute* _attribute)
{
    if (_attribute != myAttributePtr || myType != _attribute->GetType())
        RebuildAttributeWidget(_attribute);

    switch(myType)
    {
        case AttributeTypeHelper::Type::Array :
        {
            auto* qarray = dynamic_cast<QArrayLabel*>(myContent);
            const AArray* arrayAttribute = dynamic_cast<const AArray*>(_attribute);
            if(!qarray || !arrayAttribute)
            {
                LogErrorCast();
                return;
            }

            qarray->SetValue(arrayAttribute);
            break;
        }
        case AttributeTypeHelper::Type::Structure :
        {
            auto* qastructure = dynamic_cast<QAStructureLabel*>(myContent);
            const AStructure* structAttribute = dynamic_cast<const AStructure*>(_attribute);
            if(!qastructure || !structAttribute)
            {
                LogErrorCast();
                return;
            }

            qastructure->SetValue(structAttribute->GetAttributes());
            break;
        }
        case AttributeTypeHelper::Type::Bool :
        {
            auto* checkBox = dynamic_cast<QCheckBox*>(myContent);
            const ABool* boolAttribute = dynamic_cast<const ABool*>(_attribute);
            if(!checkBox || !boolAttribute)
            {
                LogErrorCast();
                return;
            }

            checkBox->setChecked(boolAttribute->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::Enum :
        {
            auto* comboBox = dynamic_cast<QComboBox*>(myContent);
            const AEnumerator* enumeratorAttribute = dynamic_cast<const AEnumerator*>(_attribute);
            if(!comboBox || !enumeratorAttribute)
            {
                LogErrorCast();
                return;
            }

            QObject::disconnect(comboBox, nullptr, nullptr, nullptr);
            while (comboBox->count() > 0)
            {
                comboBox->removeItem(0);
            }
            const auto* attrEnum = enumeratorAttribute->GetEnum();
            if (!attrEnum)
                break;
            const int valueCount = attrEnum->GetValueCount();
            int currIndex = enumeratorAttribute->GetEnumValue();
            for (int i = 0; i < valueCount; i++)
            {
                comboBox->addItem(attrEnum->GetValue(i));
            }
            if (currIndex >= valueCount)
                currIndex = 0;
            comboBox->setCurrentIndex(currIndex);
            attrEnum->SetColorToWidget(currIndex, comboBox);
            QObject::connect(comboBox, &QComboBox::currentIndexChanged,
                                 this, &QAttribute::ContentStateChanged);
            break;
        }
        case AttributeTypeHelper::Type::Float :
        {
            auto* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(myContent);
            const AFloat* floatAttribute = dynamic_cast<const AFloat*>(_attribute);
            if(!doubleSpinBox || !floatAttribute)
            {
                LogErrorCast();
                return;
            }

            bool use;
            float max = floatAttribute->GetMax(use);
            doubleSpinBox->setMaximum(use ? max : INT_MAX);
            float min = floatAttribute->GetMin(use);
            doubleSpinBox->setMinimum(use ? min : INT_MIN);
            doubleSpinBox->setValue(floatAttribute->GetValue(false));
            break;
        }
        case AttributeTypeHelper::Type::Int :
        {
            auto* spinBox = dynamic_cast<QSpinBox*>(myContent);
            const AInt* intAttribute = dynamic_cast<const AInt*>(_attribute);
            if(!spinBox || !intAttribute)
            {
                LogErrorCast();
                return;
            }

            bool use;
            int max = intAttribute->GetMax(use);
            spinBox->setMaximum(use ? max : INT_MAX);
            int min = intAttribute->GetMin(use);
            spinBox->setMinimum(use ? min : INT_MIN);
            spinBox->setValue(intAttribute->GetValue(false));
            break;
        }
        case AttributeTypeHelper::Type::ShortString :
        {
            auto* lineEdit = dynamic_cast<QLineEdit*>(myContent);
            if(!lineEdit)
            {
                LogErrorCast();
                return;
            }

            lineEdit->setText(_attribute->GetDisplayedText());
            break;
        }
        case AttributeTypeHelper::Type::TableString :
        {
            auto* qsstring = dynamic_cast<QSString*>(myContent);
            const ATableString* stringAttribute = dynamic_cast<const ATableString*>(_attribute);
            if(!qsstring || !stringAttribute)
            {
                LogErrorCast();
                return;
            }

            qsstring->SetValue(stringAttribute->GetTableName(), stringAttribute->GetStringIdentifier());
            break;
        }
        case AttributeTypeHelper::Type::Texture :
        {
            auto* qassetTexture = dynamic_cast<QAssetTexture*>(myContent);
            if(!qassetTexture)
            {
                LogErrorCast();
                return;
            }

            qassetTexture->SetValue(_attribute->GetValueAsText());
            break;
        }
        case AttributeTypeHelper::Type::UAsset :
        case AttributeTypeHelper::Type::Mesh :
        case AttributeTypeHelper::Type::Niagara :
        case AttributeTypeHelper::Type::Sound :
        {
            auto* qasset = dynamic_cast<QAssetLabel*>(myContent);
            if(!qasset)
            {
                LogErrorCast();
                return;
            }

            qasset->SetValue(_attribute->GetValueAsText());
            break;
        }
        case AttributeTypeHelper::Type::Reference :
        {
            QRefLabel* refLabel = dynamic_cast<QRefLabel*>(myContent);
            const AReference* refAttribute = dynamic_cast<const AReference*>(_attribute);
            if(!refLabel || !refAttribute)
            {
                LogErrorCast();
                return;
            }

            refLabel->SetStructureDB(refAttribute->GetStructureDB());
            refLabel->SetValue(refAttribute->GetReferenceIndex());
            break;
        }
        default:
            break;
    }

    update();
}

void QAttribute::ContentStateChanged()
{
    qDebug() << "Attribute update with type " << AttributeTypeHelper::TypeToString(myType) << " (" << (int)myType << ')';

    QString valueString;
    switch(myType)
    {
        case AttributeTypeHelper::Type::Array :
        {
            auto* arrayLabel = dynamic_cast<QArrayLabel*>(myContent);
            if(!arrayLabel)
            {
                LogErrorCast();
                return;
            }
            valueString = arrayLabel->GetValue();
            break;
        }
        case AttributeTypeHelper::Type::Structure :
        {
            auto* structLabel = dynamic_cast<QAStructureLabel*>(myContent);
            if(!structLabel)
            {
                LogErrorCast();
                return;
            }
            valueString = structLabel->GetValue();
            break;
        }
        case AttributeTypeHelper::Type::Bool :
        {
            auto* checkBox = dynamic_cast<QCheckBox*>(myContent);
            if(!checkBox)
            {
                LogErrorCast();
                return;
            }

            valueString = checkBox->isChecked() ? "true" : "false";
            break;
        }
        case AttributeTypeHelper::Type::Enum :
        {
            auto* comboBox = dynamic_cast<QComboBox*>(myContent);
            if(!comboBox)
            {
                LogErrorCast();
                return;
            }
            valueString = comboBox->currentText();
            break;
        }
        case AttributeTypeHelper::Type::Float :
        case AttributeTypeHelper::Type::Int :
        {
            auto* spinBoxAbstr = dynamic_cast<QAbstractSpinBox*>(myContent);
            if(!spinBoxAbstr)
            {
                LogErrorCast();
                return;
            }
            valueString = spinBoxAbstr->text();
            valueString.replace(',', '.');
            break;
        }
        case AttributeTypeHelper::Type::ShortString :
        {
            auto* lineEdit = dynamic_cast<QLineEdit*>(myContent);
            if(!lineEdit)
            {
                LogErrorCast();
                return;
            }
            valueString = lineEdit->text();
            break;
        }
        case AttributeTypeHelper::Type::TableString :
        {
            auto* qsstring = dynamic_cast<QSString*>(myContent);
            if(!qsstring)
            {
                LogErrorCast();
                return;
            }
            valueString = qsstring->GetFormattedValue();
            break;
        }
        case AttributeTypeHelper::Type::Texture :
        {
            auto* qassetTexture = dynamic_cast<QAssetTexture*>(myContent);
            if(!qassetTexture)
            {
                LogErrorCast();
                return;
            }
            valueString = qassetTexture->GetValue();
            break;
        }
        case AttributeTypeHelper::Type::UAsset :
        case AttributeTypeHelper::Type::Mesh :
        case AttributeTypeHelper::Type::Niagara :
        case AttributeTypeHelper::Type::Sound :
        {
            auto* qasset = dynamic_cast<QAssetLabel*>(myContent);
            if(!qasset)
            {
                LogErrorCast();
                return;
            }
            valueString = qasset->GetValue();
            break;
        }
        case AttributeTypeHelper::Type::Reference :
        {
            QRefLabel* refLabel = dynamic_cast<QRefLabel*>(myContent);
            if(!refLabel)
            {
                LogErrorCast();
                return;
            }
            valueString = refLabel->GetValueText();
            break;
        }
        default:
            break;
    }

    emit OnWidgetValueChanged(valueString);
}
void QAttribute::EmptyArrayAttribute()
{
    if (myType != AttributeTypeHelper::Type::Array)
        return;

    emit OnWidgetValueChanged("[]");
}
