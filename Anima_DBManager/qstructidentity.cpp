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
    myTemplateName = new QLineEdit();
    nameLayout->addWidget(myTemplateAbbrev);
    nameLayout->addWidget(myTemplateName);
    layout->addLayout(nameLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    myIconTypeComboBox = new QComboBox();
    myIconTypeComboBox->setMaximumWidth(180);
    InitIconComboBox();
    myIconColorEditor = new QColorEditor();
    iconLayout->addWidget(myIconTypeComboBox);
    iconLayout->addWidget(myIconColorEditor);
    layout->addLayout(iconLayout);

    QObject::connect(myTemplateName, &QLineEdit::editingFinished, this, &QStructIdentity::OnNameChanged);
    QObject::connect(myTemplateAbbrev, &QLineEdit::editingFinished, this, &QStructIdentity::OnAbbrevChanged);
    QObject::connect(myIconTypeComboBox, &QComboBox::currentIndexChanged, this, &QStructIdentity::OnIconChanged);
    QObject::connect(myIconColorEditor, &QColorEditor::ColorChanged, this, &QStructIdentity::ColorChanged);
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
    blockSignals(true);
    myTemplateName->setText(_template.GetStructName());
    myTemplateAbbrev->setText(_template.GetStructAbbrev());
    myIconTypeComboBox->setCurrentIndex((int)(_template.GetStructIcon()));
    myIconColorEditor->SetColor(_template.GetStructColor());
    blockSignals(false);
}

void QStructIdentity::OnNameChanged()
{
    emit NameChanged(myTemplateName->text());
}
void QStructIdentity::OnAbbrevChanged()
{
    emit AbbrevChanged(myTemplateAbbrev->text());
}
void QStructIdentity::OnIconChanged(const int _iconTypeIndex)
{
    if (_iconTypeIndex < 0 || _iconTypeIndex >= (int)IconManager::IconType::Count) {
        qWarning("QStructIdentity -- Invalid IconType index provided on OnIconChanged(..) : ignored");
    }

    emit IconChanged((IconManager::IconType)_iconTypeIndex);
}
