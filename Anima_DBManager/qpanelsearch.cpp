#include "qpanelsearch.h"


#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>

const QString QPanelSearch::ourCountString = "ResultCount: %1";

QPanelSearch::QPanelSearch(QWidget* _parent)
    : QWidget{_parent}
{
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    QGroupBox* paramGroupBox = new QGroupBox("Search Parameters");
    QFormLayout* paramLayout = new QFormLayout(paramGroupBox);
    layout->addWidget(paramGroupBox);

    mySearchField = new QLineEdit();
    mySearchField->setPlaceholderText("Search");
    paramLayout->addWidget(mySearchField);

    mySearchOnStruct = new QCheckBox();
    mySearchOnAttributeGroup = new QGroupBox("Attributes");
    mySearchOnAttributeGroup->hide();
    QGridLayout* attributeGroupLayout = new QGridLayout(mySearchOnAttributeGroup);
#define ADD_ATTRIBUTE_CHECKBOX(type, row, col) \
    QCheckBox* checkBox##type = new QCheckBox(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::type)); \
    QObject::connect(checkBox##type, &QCheckBox::stateChanged, this, [this](int _state){ OnAttributeCheckBoxChanged(_state, AttributeTypeHelper::Type::type); }); \
    checkBox##type->setCheckState(Qt::Checked); \
    attributeGroupLayout->addWidget(checkBox##type, row, col);

    ADD_ATTRIBUTE_CHECKBOX(Bool, 0, 0);
    ADD_ATTRIBUTE_CHECKBOX(Int, 1, 0);
    ADD_ATTRIBUTE_CHECKBOX(Float, 2, 0);
    ADD_ATTRIBUTE_CHECKBOX(ShortString, 3, 0);

    ADD_ATTRIBUTE_CHECKBOX(Enum, 0, 1);
    ADD_ATTRIBUTE_CHECKBOX(TableString, 1, 1);
    ADD_ATTRIBUTE_CHECKBOX(Reference, 2, 1);

    ADD_ATTRIBUTE_CHECKBOX(Array, 0, 2);
    ADD_ATTRIBUTE_CHECKBOX(Structure, 1, 2);

    ADD_ATTRIBUTE_CHECKBOX(UAsset, 0, 3);
    ADD_ATTRIBUTE_CHECKBOX(Texture, 1, 3);
    ADD_ATTRIBUTE_CHECKBOX(Mesh, 2, 3);
    ADD_ATTRIBUTE_CHECKBOX(Niagara, 3, 3);
    ADD_ATTRIBUTE_CHECKBOX(Sound, 4, 3);

#undef ADD_ATTRIBUTE_CHECKBOX

    mySearchOnString = new QCheckBox();
    mySearchOnLanguageGroup = new QGroupBox("Languages");
    mySearchOnLanguageGroup->hide();
    QGridLayout* languageGroupLayout = new QGridLayout(mySearchOnLanguageGroup);
#define ADD_LANGUAGE_CHECKBOX(language, row, col) \
    QCheckBox* checkBox##language = new QCheckBox(SStringHelper::GetLanguageString(SStringHelper::SStringLanguages::language)); \
    QObject::connect(checkBox##language, &QCheckBox::stateChanged, this, [this](int _state){ OnLanguageCheckBoxChanged(_state, SStringHelper::SStringLanguages::language); }); \
    checkBox##language->setCheckState(Qt::Unchecked); \
    languageGroupLayout->addWidget(checkBox##language, row, col);

    ADD_LANGUAGE_CHECKBOX(French, 0, 0);
    ADD_LANGUAGE_CHECKBOX(English, 0, 1);
    checkBoxFrench->setCheckState(Qt::Checked);
#undef ADD_ATTRIBUTE_CHECKBOX

    mySearchOnEnum = new QCheckBox();
    paramLayout->addRow("Search on Structure:", mySearchOnStruct);
    paramLayout->addWidget(mySearchOnAttributeGroup);
    paramLayout->addRow("Search on Strings:", mySearchOnString);
    paramLayout->addWidget(mySearchOnLanguageGroup);
    paramLayout->addRow("Search on Enums:", mySearchOnEnum);
    QPushButton* searchBtn = new QPushButton("Search");
    paramLayout->addWidget(searchBtn);

    QGroupBox* resultGroupBox = new QGroupBox("Search Result");
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroupBox);
    layout->addWidget(resultGroupBox);

    myResultTable = new QTableWidget();
    myResultTable->setColumnCount(5);
    QStringList headers = QStringList();
    headers.append("Origin");
    headers.append("Table");
    headers.append("Item");
    headers.append("Attribute/Language");
    headers.append("Context");
    myResultTable->setHorizontalHeaderLabels(headers);
    resultLayout->addWidget(myResultTable);

    myResultCount = new QLabel(ourCountString.arg(""));
    resultLayout->addWidget(myResultCount);


    QObject::connect(mySearchOnStruct, &QCheckBox::stateChanged, this, &QPanelSearch::OnStructCheckboxChanged);
    QObject::connect(mySearchOnString, &QCheckBox::stateChanged, this, &QPanelSearch::OnStringCheckboxChanged);
    QObject::connect(mySearchOnEnum, &QCheckBox::stateChanged, this, &QPanelSearch::OnEnumCheckboxChanged);
}

void QPanelSearch::OnStructCheckboxChanged(int _state)
{
    mySearchParameters.mySearchOnStructs = _state != (int)Qt::Unchecked;
    if (mySearchParameters.mySearchOnStructs)
    {
        mySearchOnAttributeGroup->show();
    }
    else
    {
        mySearchOnAttributeGroup->hide();
    }
}
void QPanelSearch::OnStringCheckboxChanged(int _state)
{
    mySearchParameters.mySearchOnStrings = _state != (int)Qt::Unchecked;
    if (mySearchParameters.mySearchOnStrings)
    {
        mySearchOnLanguageGroup->show();
    }
    else
    {
        mySearchOnLanguageGroup->hide();
    }
}
void QPanelSearch::OnEnumCheckboxChanged(int _state)
{
    mySearchParameters.mySearchOnEnum = _state != (int)Qt::Unchecked;
}
void QPanelSearch::OnAttributeCheckBoxChanged(int _state, AttributeTypeHelper::Type _type)
{
    mySearchParameters.myAttributeIgnoreSearchMap.insert(_type, _state != (int)Qt::Unchecked);
}
void QPanelSearch::OnLanguageCheckBoxChanged(int _state, SStringHelper::SStringLanguages _language)
{
    mySearchParameters.myLanguageIgnoreSearchMap.insert(_language, _state != (int)Qt::Unchecked);
}
