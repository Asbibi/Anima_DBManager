#include "qstructidentity.h"

#include "iconmanager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

QStructIdentity::QStructIdentity(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    myTemplateAbbrev = new QLineEdit();
    myTemplateAbbrev->setMaximumWidth(90);
    myTemplatename = new QLineEdit();
    nameLayout->addWidget(myTemplateAbbrev);
    nameLayout->addWidget(myTemplatename);
    layout->addLayout(nameLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    myIconTypeComboBox = new QComboBox();
    myIconTypeComboBox->setMaximumWidth(180);
    InitIconComboBox();
    myIconColorEditor = new QColorEditor();
    iconLayout->addWidget(myIconTypeComboBox);
    iconLayout->addWidget(myIconColorEditor);
    layout->addLayout(iconLayout);
}
void QStructIdentity::InitIconComboBox()
{
#define ADD_ICON_OPTION(Option) myIconTypeComboBox->addItem(IconManager::GetSimpleIcon(IconManager::IconType::Option, QColorConstants::Black), #Option);

    ADD_ICON_OPTION(Square);
    ADD_ICON_OPTION(SquareHollow);
    ADD_ICON_OPTION(Circle);
    ADD_ICON_OPTION(CircleHollow);
    ADD_ICON_OPTION(TriangleUp);
    ADD_ICON_OPTION(TriangleUpHollow);
    ADD_ICON_OPTION(TriangleDown);
    ADD_ICON_OPTION(TriangleDownHollow);
    ADD_ICON_OPTION(Star);
    ADD_ICON_OPTION(StarHollow);
    ADD_ICON_OPTION(V);
    ADD_ICON_OPTION(X);
    ADD_ICON_OPTION(Anima);
    ADD_ICON_OPTION(AnimaHollow);
    ADD_ICON_OPTION(Stuff);
    ADD_ICON_OPTION(StuffHollow);
    ADD_ICON_OPTION(Map);
    ADD_ICON_OPTION(MapHollow);
    ADD_ICON_OPTION(Scroll);
    ADD_ICON_OPTION(ScrollHollow);
    ADD_ICON_OPTION(Face);
    ADD_ICON_OPTION(FaceHollow);
    ADD_ICON_OPTION(Text1);
    ADD_ICON_OPTION(Text2);
    ADD_ICON_OPTION(Text3);
    ADD_ICON_OPTION(Text4);

#undef ADD_ICON_OPTION
}



void QStructIdentity::SetValueFromTemplate(const TemplateStructure& _template)
{
    myTemplatename->setText(_template.GetStructName());
    myTemplateAbbrev->setText(_template.GetStructAbbrev());
    myIconTypeComboBox->setCurrentIndex((int)(_template.GetStructIcon()));
    myIconColorEditor->SetColor(_template.GetStructColor());
}
