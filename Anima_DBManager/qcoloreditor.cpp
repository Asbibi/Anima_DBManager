#include "qcoloreditor.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QPalette>

QColorEditor::QColorEditor(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    myColorText = new QLineEdit();
    myColorText->setMaximumWidth(70);
    QObject::connect(myColorText, &QLineEdit::editingFinished, this, &QColorEditor::OnColorTextChanged);
    layout->addWidget(myColorText);

    QFormLayout * subLayout = new QFormLayout();
#define BUILD_SLIDER(sliderName, max, name) sliderName = new QSlider(Qt::Horizontal);\
    sliderName->setMinimum(0);\
    sliderName->setMaximum(max);\
    QObject::connect(sliderName, &QSlider::sliderMoved, this, &QColorEditor::OnSliderChanged);\
    subLayout->addRow(name, sliderName);

    BUILD_SLIDER(myHueSlider, 360, "H");
    BUILD_SLIDER(mySaturationSlider, 255, "S");
    BUILD_SLIDER(myValueSlider, 255, "V");
#undef BUILD_SLIDER
    layout->addLayout(subLayout);
    SetColor(QColorConstants::White);
}
void QColorEditor::UpdateColorText()
{
    myColorText->setText(myColor.name());

    QPalette p = myColorText->palette();
    p.setColor(myColorText->backgroundRole(), myColor);
    myColorText->setPalette(p);
}

void QColorEditor::SetColor(const QColor& _color)
{
    if (myColor == _color)
    {
        return;
    }

    myColor = _color;
    myHueSlider->setValue(myColor.hsvHue());
    mySaturationSlider->setValue(myColor.hsvSaturation());
    myValueSlider->setValue(myColor.value());
    UpdateColorText();
}
const QColor& QColorEditor::GetColor() const
{
    return myColor;
}

void QColorEditor::OnSliderChanged()
{
    myColor.setHsv(myHueSlider->value(), mySaturationSlider->value(), myValueSlider->value());
    UpdateColorText();
}
void QColorEditor::OnColorTextChanged()
{
    const QString& colorHex = myColorText->text();
    if (!QColor::isValidColor(colorHex))
    {
        myColorText->setText(myColor.name());
        return;
    }

    SetColor(QColor(colorHex));
}
