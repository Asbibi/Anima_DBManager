#include "qpanelsearch.h"

#include "attributetype.h"

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
    mySearchOnAttributGroup = new QGroupBox("Attributes");
    mySearchOnAttributGroup->hide();
    QGridLayout* attributeGroupLayout = new QGridLayout(mySearchOnAttributGroup);
#define ADD_ATTRIBUTE_CHECKBOX(type, row, col) \
    QCheckBox* checkBox##type = new QCheckBox(AttributeTypeHelper::TypeToString(AttributeTypeHelper::Type::type)); \
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
    mySearchOnEnum = new QCheckBox();
    paramLayout->addRow("Search on Structure:", mySearchOnStruct);
    paramLayout->addWidget(mySearchOnAttributGroup);
    paramLayout->addRow("Search on Strings:", mySearchOnString);
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
}

void QPanelSearch::OnStructCheckboxChanged(int _state)
{
    if (_state == (int)Qt::Unchecked)
    {
        mySearchOnAttributGroup->hide();
    }
    else
    {
        mySearchOnAttributGroup->show();
    }
}
