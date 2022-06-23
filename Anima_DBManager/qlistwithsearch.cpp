#include "qlistwithsearch.h"

#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>

QListWithSearch::QListWithSearch(bool _unique, const QString& _defValue, QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);

    mySearchField = new QLineEdit();
    mySearchField->setStyleSheet(
                "background-color: #FFD125;"
                "border-style: outset;"
                "border-width: 1px;"
                "border-radius: 10px;"
                "border-color: black;"
                "font: italic 11px;");
    mySearchField->setPlaceholderText("Search Identifier");
    myLayout->addWidget(mySearchField);


    myStackedWidget = new QStackedWidget();
    myLayout->addWidget(myStackedWidget);

    myOriginalList = new QAugmentedList(_unique, _defValue);
    myStackedWidget->addWidget(myOriginalList);
    mySearchList = new QListWidget();
    mySearchList->setStyleSheet("background-color: #FFF1BD;");
    myStackedWidget->addWidget(mySearchList);


    QObject::connect(mySearchField, &QLineEdit::textEdited, this, &QListWithSearch::OnSearchFieldChanged);
    QObject::connect(mySearchList, &QListWidget::currentTextChanged, this, &QListWithSearch::OnSearchItemSelected);

#define CONNECT_ORIGINAL(method) QObject::connect(myOriginalList, &QAugmentedList::method, this, &QListWithSearch::OnOriginal##method)
    CONNECT_ORIGINAL(SelectionChanged);
    CONNECT_ORIGINAL(ItemEdited);
    CONNECT_ORIGINAL(ItemAdded);
    CONNECT_ORIGINAL(ItemDuplicated);
    CONNECT_ORIGINAL(ItemMoved);
    CONNECT_ORIGINAL(ItemRemoved);
#undef CONNECT_ORIGINAL
}

void QListWithSearch::SetDisableRemoveLast(bool _can)
{
    myOriginalList->SetDisableRemoveLast(_can);
}



void QListWithSearch::ResetValues()
{
    myOriginalList->Clear();
    mySearchField->clear();
    OnSearchFieldChanged("");
}

void QListWithSearch::AddItemAtEnd_NoEmit(const QString& _label)
{
    myOriginalList->AddItemAt(_label);
}



void QListWithSearch::OnOriginalSelectionChanged(const int _index) { emit SelectionChanged(_index); }
void QListWithSearch::OnOriginalItemEdited(const int _index, const QString& _value) { emit ItemEdited(_index, _value); }
void QListWithSearch::OnOriginalItemAdded(const int _index, const QString& _value) { emit ItemAdded(_index, _value); }
void QListWithSearch::OnOriginalItemDuplicated(const int _index, const int _originalIndex) { emit ItemDuplicated(_index, _originalIndex); }
void QListWithSearch::OnOriginalItemMoved(const int _indexFrom, const int _indexTo) { emit ItemMoved(_indexFrom, _indexTo); }
void QListWithSearch::OnOriginalItemRemoved(const int _index) { emit ItemRemoved(_index); }


void QListWithSearch::OnSearchFieldChanged(const QString& text)
{
    if (text.isEmpty())
    {
        myStackedWidget->setCurrentIndex(0);
        return;
    }

    mySearchList->clear();
    const int count = myOriginalList->Count();
    for (int i = 0; i < count; i++)
    {
        const QString value_i = myOriginalList->GetItemValue(i);
        if (value_i.contains(text))
        {
            mySearchList->addItem(value_i);
        }
    }
    myStackedWidget->setCurrentIndex(1);
}

void QListWithSearch::OnSearchItemSelected(const QString& text)
{
    myOriginalList->SelectItem(text);
}
