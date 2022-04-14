#include "qattribute.h"

#include <QCheckBox>
#include <QTextEdit>

QAttribute::QAttribute(QWidget* parent) :
    QWidget(parent)
{
//    resize(param.hSize, param.vSize);

//    QPalette pal = QPalette();
//    pal.setColor(QPalette::Window, param.backColor);
//    setAutoFillBackground(true);
//    setPalette(pal);
}

QAttribute::~QAttribute()
{
    //....
}

void QAttribute::UpdateAttribute(const Attribute& _attribute)
{
    //switch sur le type de l'attribute et selon myType

    // Label
//    myLabel = new QTextEdit(this);
//    myLabel->setText(myAttribute->GetDisplayedText(false));
//    if (_redraw)
//        update();

    // Checkbox
//    myCheckBox = new QCheckBox(this);
//    myCheckBox->setChecked(myAttribute->GetDisplayedText(false)[0] == 'T');
//    if (_redraw)
//        update();
}
