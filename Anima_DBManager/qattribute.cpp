#include "qattribute.h"

#include "abool.h"
#include "aenumerator.h"

#include <QDebug>
#include <QtGlobal>

#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

QAttribute::QAttribute(QWidget* parent) :
    QWidget(parent)
{
    setLayout(new QVBoxLayout());
    resize(100,100);
//    resize(param.hSize, param.vSize);

//    QPalette pal = QPalette();
//    pal.setColor(QPalette::Window, param.backColor);
//    setAutoFillBackground(true);
//    setPalette(pal);
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
    if (myContent == nullptr)
    {
        return;
    }

    delete myContent;
    myContent = nullptr;
}

void QAttribute::RebuildAttributeWidget(const Attribute* _attribute)
{
    // We assume that myAttributePtr != _attribute, so need to reconnect to the right attribute object
    QObject::disconnect(this, nullptr, nullptr, nullptr);
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
            auto* content = new QLabel(this);
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
            //myContent->resize(20,20);
            break;
        }
        case Attribute::Type::Enum :
        {
            auto* content = new QComboBox(this);
            QObject::connect(content, &QComboBox::currentIndexChanged,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::Float :
        {
            auto* content = new QLineEdit(this);
            QObject::connect(content, &QLineEdit::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::Int :
        {
            auto* content = new QLineEdit(this);
            QObject::connect(content, &QLineEdit::editingFinished,
                                 this, &QAttribute::ContentStateChanged);
            myContent = content;
            break;
        }
        case Attribute::Type::String :
        {
            auto* content = new QLabel(this);   // As there will be a table for string, need to think of how to set a string identifier, evnetually create an alternative attribute subclass for short untranslatable strings (names ?) that won't use the identifier ?
            myContent = content;
            break;
        }
        case Attribute::Type::Structure :
        {
            auto* content = new QLabel(this);
            myContent = content;
            break;
        }
    }

    if (myContent != nullptr)
    {
        layout()->addWidget(myContent);
    }
}

void QAttribute::UpdateAttribute(const Attribute* _attribute)
{
    if (_attribute != myAttributePtr)
        RebuildAttributeWidget(_attribute);

    switch(myType)
    {
        case Attribute::Type::Array :
        case Attribute::Type::Structure :
        case Attribute::Type::String :
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

            break;
        }
        case Attribute::Type::Float :
        case Attribute::Type::Int :
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
    }

    update();
}

void QAttribute::ContentStateChanged()//Attribute& _attribute)
{
    qDebug() << "Attribute update with type " << (int)myType;

    QString valueString;
    switch(myType)
    {
        case Attribute::Type::Array :
        case Attribute::Type::Structure :
        case Attribute::Type::String :
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
            //enumeratorAttribute->SetEnumValue(comboBox->currentIndex());
            break;
        }
        case Attribute::Type::Float :
        case Attribute::Type::Int :
        {
            auto* lineEdit = dynamic_cast<QLineEdit*>(myContent);
            if(!lineEdit)
            {
                LogErrorCast();
                return;
            }

            valueString = lineEdit->text();
            //_attribute.SetValueFromText(lineEdit->text());
            break;
        }
    }

    emit OnWidgetValueChanged(valueString);
}
