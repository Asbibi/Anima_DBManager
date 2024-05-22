#include "qoptionalvalue.h"

#include <QHBoxLayout>


QOptionalValue_Int::QOptionalValue_Int(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* myLayout = new QHBoxLayout();
    setLayout(myLayout);
    myLayout->setContentsMargins(0,0,0,0);

    myCheckBox = new QCheckBox();
    myLayout->addWidget(myCheckBox);
    QObject::connect(myCheckBox, &QCheckBox::stateChanged, this, &QOptionalValue_Int::MyCheckBoxChanged);

    mySpinBox = new QSpinBox();
    mySpinBox->setMinimum(INT_MIN);
    mySpinBox->setMaximum(INT_MAX);
    myLayout->addWidget(mySpinBox);
    QObject::connect(mySpinBox, &QSpinBox::valueChanged, this, &QOptionalValue_Int::MyValueChanged);
}

void QOptionalValue_Int::SetValues(bool _enabled, int _value)
{
    myCheckBox->setChecked(_enabled);
    mySpinBox->setEnabled(_enabled);
    mySpinBox->setValue(_value);
}

void QOptionalValue_Int::SetMinimum(int _value)
{
    mySpinBox->setMinimum(_value);
}

void QOptionalValue_Int::MyCheckBoxChanged(int _state)
{
    bool checked = _state != Qt::Unchecked;
    mySpinBox->setEnabled(checked);
    emit OnEnableChanged(checked);
}

void QOptionalValue_Int::MyValueChanged(int _value)
{
    emit OnValueChanged(_value);
}




QOptionalValue_Float::QOptionalValue_Float(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* myLayout = new QHBoxLayout();
    setLayout(myLayout);
    myLayout->setContentsMargins(0,0,0,0);

    myCheckBox = new QCheckBox();
    myLayout->addWidget(myCheckBox);
    QObject::connect(myCheckBox, &QCheckBox::stateChanged, this, &QOptionalValue_Float::MyCheckBoxChanged);

    mySpinBox = new QDoubleSpinBox();
    mySpinBox->setMinimum(INT_MIN);
    mySpinBox->setMaximum(INT_MAX);
    myLayout->addWidget(mySpinBox);
    QObject::connect(mySpinBox, &QDoubleSpinBox::valueChanged, this, &QOptionalValue_Float::MyValueChanged);
}

void QOptionalValue_Float::SetValues(bool _enabled, float _value)
{
    myCheckBox->setChecked(_enabled);
    mySpinBox->setEnabled(_enabled);
    mySpinBox->setValue(_value);
}

void QOptionalValue_Float::SetMinimum(float _value)
{
    mySpinBox->setMinimum(_value);
}

void QOptionalValue_Float::MyCheckBoxChanged(int _state)
{
    bool checked = _state != Qt::Unchecked;
    mySpinBox->setEnabled(checked);
    emit OnEnableChanged(checked);
}

void QOptionalValue_Float::MyValueChanged(float _value)
{
    emit OnValueChanged(_value);
}
