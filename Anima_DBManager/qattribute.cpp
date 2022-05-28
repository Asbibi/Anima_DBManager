#include "qattribute.h"

#include "abool.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "atablestring.h"
#include "qsstring.h"

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

    const Attribute::Type _newType = _attribute ? _attribute->GetType() : Attribute::Type::Invalid;
    if (_newType != myType)
    {
        RebuildWidgetFromType(_newType);
    }

    myAttributePtr = _attribute;
    myType = _newType;
}

void QAttribute::RebuildWidgetFromType(const Attribute::Type _type)
{
    DeleteContent();

    switch(_type)
    {
        case Attribute::Type::Array :
        {
            QLabel* content = new QLabel(this);
            myContent = content;
            break;
        }
        case Attribute::Type::Bool :
        {
            QCheckBox* content = new QCheckBox(this);
            QObject::connect(content, &QCheckBox::clicked,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            layout()->addWidget(myContent);
            break;
        }
        case Attribute::Type::Enum :
        {
            QComboBox* content = new QComboBox(this);
            // Connection Handled in Update since it needs to be temporary disabled
            myContent = content;
            break;
        }
        case Attribute::Type::Float :
        {
            QDoubleSpinBox* content = new QDoubleSpinBox(this);
            QObject::connect(content, &QDoubleSpinBox::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::Int :
        {
            QSpinBox* content = new QSpinBox(this);
            QObject::connect(content, &QSpinBox::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::ShortString :
        {
            QLineEdit* content = new QLineEdit(this);
            QObject::connect(content, &QLineEdit::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::TableString :
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
        case Attribute::Type::Structure :
        {
            QLabel* content = new QLabel(this);
            myContent = content;
            break;
        }
    }

    if (myContent != nullptr)
    {
        layout()->addWidget(myContent);
    }
    if (myEditButton != nullptr)
    {
        layout()->addItem(new QSpacerItem(20,5));
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
    if (_attribute != myAttributePtr)
        RebuildAttributeWidget(_attribute);

    switch(myType)
    {
        case Attribute::Type::Array :
        case Attribute::Type::Structure :
        {
            auto* label = dynamic_cast<QLabel*>(myContent);
            if(!label)
            {
                LogErrorCast();
                return;
            }

            label->setText(_attribute->GetDisplayedText());
            break;
        }
        case Attribute::Type::Bool :
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
        case Attribute::Type::Enum :
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
            const int valueCount = attrEnum->GetValueCount();
            const int currIndex = enumeratorAttribute->GetEnumValue();
            for (int i = 0; i < valueCount; i++)
            {
                comboBox->addItem(attrEnum->GetValue(i));
            }
            comboBox->setCurrentIndex(currIndex);
            attrEnum->SetColorToWidget(currIndex, comboBox);
            QObject::connect(comboBox, &QComboBox::currentIndexChanged,
                                 this, &QAttribute::ContentStateChanged);
            break;
        }
        case Attribute::Type::Float :
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
        case Attribute::Type::Int :
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
        case Attribute::Type::ShortString :
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
        case Attribute::Type::TableString :
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
    }

    update();
}

void QAttribute::ContentStateChanged()
{
    qDebug() << "Attribute update with type " << (int)myType;

    QString valueString;
    switch(myType)
    {
        case Attribute::Type::Array :
        case Attribute::Type::Structure :
        {
            break;
        }
        case Attribute::Type::Bool :
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
        case Attribute::Type::Enum :
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
        case Attribute::Type::Float :
        case Attribute::Type::Int :
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
        case Attribute::Type::ShortString :
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
        case Attribute::Type::TableString :
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
    }

    emit OnWidgetValueChanged(valueString);
}
