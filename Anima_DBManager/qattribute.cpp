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
#include "db_manager.h"
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
    QObject::connect(this, &QAttribute::OnWidgetValueChanged, _attribute, &Attribute::SetValue_JSON);

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

    const auto& dbManager = DB_Manager::GetDB_Manager();
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
#define CASE_ATTRIBUTE(type, typeStr, typeFile, ext) \
case AttributeTypeHelper::Type::type : \
{ \
    QAssetLabel* content = new QAssetLabel(AttributeTypeHelper::Type::type, typeStr, typeFile" (" + dbManager.GetAAssetRegex(AttributeTypeHelper::Type::type) + "." + ext + ")", this); \
    QObject::connect(content, &QAssetLabel::OnValueEdited, \
                         this, &QAttribute::ContentStateChanged); \
    myContent = content;    \
\
    BuildMoreButton(); \
    QObject::connect(myEditButton, &QPushButton::clicked, \
                         content, &QAssetLabel::OpenFileDialog); \
    break; \
}

        CASE_ATTRIBUTE(UAsset, "UAsset", "Unreal Assets", AAsset::GetStaticAssetFileExtension());
#ifdef USE_SPECIFIC_FILE_EXTENSION_FOR_AAMESH
        CASE_ATTRIBUTE(SkeletalMesh, "SkeletalMesh", "3D Rigged Files", AAMesh::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(StaticMesh, "StaticMesh", "3D Static Files", AAMesh::GetStaticAssetFileExtension());
#else
        CASE_ATTRIBUTE(SkeletalMesh, "SkeletalMesh", "3D Skeletal Meshes", AAsset::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(StaticMesh, "StaticMesh", "3D Static Meshes", AAsset::GetStaticAssetFileExtension());
#endif
        CASE_ATTRIBUTE(Niagara, "Niagara", "Niagara Systems", AAsset::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(Sound, "Sound", "Sounds", AASound::GetStaticAssetFileExtension());
        CASE_ATTRIBUTE(Class, "Class", "Blueprint Classes", AAsset::GetStaticAssetFileExtension());
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
            Q_ASSERT(qarray && arrayAttribute);

            qarray->SetValue(arrayAttribute);
            break;
        }
        case AttributeTypeHelper::Type::Structure :
        {
            auto* qastructure = dynamic_cast<QAStructureLabel*>(myContent);
            const AStructure* structAttribute = dynamic_cast<const AStructure*>(_attribute);
            Q_ASSERT(qastructure && structAttribute);

            qastructure->SetValue(structAttribute->GetAttributes());
            break;
        }
        case AttributeTypeHelper::Type::Bool :
        {
            auto* checkBox = dynamic_cast<QCheckBox*>(myContent);
            const ABool* boolAttribute = dynamic_cast<const ABool*>(_attribute);
            Q_ASSERT(checkBox && boolAttribute);

            checkBox->setChecked(boolAttribute->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::Enum :
        {
            auto* comboBox = dynamic_cast<QComboBox*>(myContent);
            const AEnumerator* enumeratorAttribute = dynamic_cast<const AEnumerator*>(_attribute);
            Q_ASSERT(comboBox && enumeratorAttribute);

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
            Q_ASSERT(doubleSpinBox && floatAttribute);

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
            Q_ASSERT(spinBox && intAttribute);

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
            Q_ASSERT(lineEdit);

            lineEdit->setText(_attribute->GetDisplayedText());
            break;
        }
        case AttributeTypeHelper::Type::TableString :
        {
            auto* qsstring = dynamic_cast<QSString*>(myContent);
            const ATableString* stringAttribute = dynamic_cast<const ATableString*>(_attribute);
            Q_ASSERT(qsstring && stringAttribute);

            qsstring->SetValue(stringAttribute->GetTableName(), stringAttribute->GetStringIdentifier());
            break;
        }
        case AttributeTypeHelper::Type::Texture :
        {
            auto* qassetTexture = dynamic_cast<QAssetTexture*>(myContent);
            const AAsset* assetAttribute = dynamic_cast<const AAsset*>(_attribute);
            Q_ASSERT(qassetTexture && assetAttribute);

            qassetTexture->SetValue(assetAttribute->GetFilePath());
            break;
        }
        case AttributeTypeHelper::Type::UAsset :
        case AttributeTypeHelper::Type::SkeletalMesh :
        case AttributeTypeHelper::Type::StaticMesh :
        case AttributeTypeHelper::Type::Niagara :
        case AttributeTypeHelper::Type::Sound :
        case AttributeTypeHelper::Type::Class :
        {
            auto* qasset = dynamic_cast<QAssetLabel*>(myContent);
            const AAsset* assetAttribute = dynamic_cast<const AAsset*>(_attribute);
            Q_ASSERT(qasset && assetAttribute);

            qasset->SetValue(assetAttribute->GetFilePath());
            break;
        }
        case AttributeTypeHelper::Type::Reference :
        {
            QRefLabel* refLabel = dynamic_cast<QRefLabel*>(myContent);
            const AReference* refAttribute = dynamic_cast<const AReference*>(_attribute);
            Q_ASSERT(refLabel && refAttribute);

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

    QJsonValue value;
    switch(myType)
    {
        case AttributeTypeHelper::Type::Array :
        {
            auto* arrayLabel = dynamic_cast<QArrayLabel*>(myContent);
            Q_ASSERT(arrayLabel);

            value = QJsonValue(arrayLabel->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::Structure :
        {
            auto* structLabel = dynamic_cast<QAStructureLabel*>(myContent);
            Q_ASSERT(structLabel);

            value = QJsonValue(structLabel->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::Bool :
        {
            auto* checkBox = dynamic_cast<QCheckBox*>(myContent);
            Q_ASSERT(checkBox);

            value = QJsonValue(checkBox->isChecked());
            break;
        }
        case AttributeTypeHelper::Type::Enum :
        {
            auto* comboBox = dynamic_cast<QComboBox*>(myContent);
            Q_ASSERT(comboBox);

            value = QJsonValue(comboBox->currentText());
            break;
        }

#define GET_SPIN_BOX_VALUE(SpinBoxClass) auto* spinBox = dynamic_cast<SpinBoxClass*>(myContent); \
    Q_ASSERT(spinBox); \
    value = QJsonValue(spinBox->value());

        case AttributeTypeHelper::Type::Float :
        {
            GET_SPIN_BOX_VALUE(QDoubleSpinBox);
            break;
        }
        case AttributeTypeHelper::Type::Int :
        {
            GET_SPIN_BOX_VALUE(QSpinBox);
            break;
        }
#undef GET_SPIN_BOX_VALUE

        case AttributeTypeHelper::Type::ShortString :
        {
            auto* lineEdit = dynamic_cast<QLineEdit*>(myContent);
            Q_ASSERT(lineEdit);

            value = QJsonValue(lineEdit->text());
            break;
        }
        case AttributeTypeHelper::Type::TableString :
        {
            auto* qsstring = dynamic_cast<QSString*>(myContent);
            Q_ASSERT(qsstring);

            QString tableId;
            QString stringId;
            qsstring->GetValue(tableId, stringId);

            value = ATableString::ConvertToJsonValue(tableId, stringId);
            break;
        }
        case AttributeTypeHelper::Type::Texture :
        {
            auto* qassetTexture = dynamic_cast<QAssetTexture*>(myContent);
            Q_ASSERT(qassetTexture);

            value = QJsonValue(qassetTexture->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::UAsset :
        case AttributeTypeHelper::Type::SkeletalMesh :
        case AttributeTypeHelper::Type::StaticMesh :
        case AttributeTypeHelper::Type::Niagara :
        case AttributeTypeHelper::Type::Sound :
        case AttributeTypeHelper::Type::Class :
        {
            auto* qasset = dynamic_cast<QAssetLabel*>(myContent);
            Q_ASSERT(qasset);

            value = QJsonValue(qasset->GetValue());
            break;
        }
        case AttributeTypeHelper::Type::Reference :
        {
            QRefLabel* refLabel = dynamic_cast<QRefLabel*>(myContent);
            Q_ASSERT(refLabel);

            value = QJsonValue(refLabel->GetValueText());
            break;
        }
        default:
            break;
    }

    emit OnWidgetValueChanged(value);
    emit DB_Manager::GetDB_Manager().AcknowledgeChange();
}
void QAttribute::EmptyArrayAttribute()
{
    if (myType != AttributeTypeHelper::Type::Array)
        return;

    emit OnWidgetValueChanged("[]");
}
