#include "qpanelsearch.h"

#include "searchmanager.h"
#include <QFormLayout>
#include <QFuture>
#include <QGridLayout>
#include <QtConcurrent>
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

    QLineEdit* searchField = new QLineEdit();
    searchField->setPlaceholderText("Search");
    paramLayout->addWidget(searchField);

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

    QCheckBox* searchOnEnum = new QCheckBox();
    QCheckBox* caseSensitive = new QCheckBox();
    QCheckBox* wholeWord = new QCheckBox();
    paramLayout->addRow("Search on Structure:", mySearchOnStruct);
    paramLayout->addWidget(mySearchOnAttributeGroup);
    paramLayout->addRow("Search on Strings:", mySearchOnString);
    paramLayout->addWidget(mySearchOnLanguageGroup);
    paramLayout->addRow("Search on Enums:", searchOnEnum);
    paramLayout->addRow("Case Sensistive:", caseSensitive);
    paramLayout->addRow("Whole word:", wholeWord);

    mySearchBtn = new QPushButton("Search");
    paramLayout->addWidget(mySearchBtn);



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


    QObject::connect(searchField, &QLineEdit::textEdited, this, &QPanelSearch::OnSearchedStringChanged);
    QObject::connect(mySearchOnStruct, &QCheckBox::stateChanged, this, &QPanelSearch::OnStructCheckboxChanged);
    QObject::connect(mySearchOnString, &QCheckBox::stateChanged, this, &QPanelSearch::OnStringCheckboxChanged);
    QObject::connect(searchOnEnum, &QCheckBox::stateChanged, this, &QPanelSearch::OnEnumCheckboxChanged);
    QObject::connect(caseSensitive, &QCheckBox::stateChanged, this, &QPanelSearch::OnCaseCheckboxChanged);
    QObject::connect(wholeWord, &QCheckBox::stateChanged, this, &QPanelSearch::OnWholeCheckboxChanged);
    QObject::connect(mySearchBtn, &QPushButton::clicked, this, &QPanelSearch::OnSearchRequested);

    //mySearchOnStruct->setCheckState(Qt::Checked);
    mySearchOnString->setCheckState(Qt::Checked);
    //searchOnEnum->setCheckState(Qt::Checked);
    //caseSensitive->setCheckState(Qt::Checked);
    //wholeWord->setCheckState(Qt::Checked);
}

void QPanelSearch::OnSearchedStringChanged(const QString& _str)
{
    mySearchParameters.mySearchedString = _str;
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
void QPanelSearch::OnCaseCheckboxChanged(int _state)
{
    mySearchParameters.myCaseSensitivity = _state != (int)Qt::Unchecked ?  Qt::CaseSensitive : Qt::CaseInsensitive;
}
void QPanelSearch::OnWholeCheckboxChanged(int _state)
{
    mySearchParameters.myWholeWord = _state != (int)Qt::Unchecked;
}

void QPanelSearch::OnSearchRequested()
{
    if (mySearchParameters.mySearchedString == "" || (mySearchParameters.myWholeWord && mySearchParameters.mySearchedString.contains(' ')))
    {
        qDebug() << "Searched with empty string or string that contains spaces with the whole word option active";
        return;
    }

    mySearchBtn->setEnabled(false);
    mySearchBtn->setText("Searching...");


    QFuture<QList<SearchResult>> futureRes = QtConcurrent::run([this] {
            return SearchManager::GetInstance().Search(mySearchParameters);
    });

    UpdateWidgetsWithSearchResults(futureRes.result());

    //QList<SearchResult> searchResults = SearchManager::GetInstance().Search(mySearchParameters);
}
void QPanelSearch::UpdateWidgetsWithSearchResults(const QList<SearchResult>& _res)
{
    mySearchBtn->setEnabled(true);
    mySearchBtn->setText("Search");

    const int resCount = _res.count();

    myResultCount->setText(ourCountString.arg(resCount));

    myResultTable->setRowCount(resCount);
    for (int i = 0; i < resCount; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            QTableWidgetItem* newItem = new QTableWidgetItem(_res[i].myDisplayString[j]);
            myResultTable->setItem(i, j, newItem);
        }
    }
}
