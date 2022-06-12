#include "qaugmentedlist.h"

#include <QPushButton>
//#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>

#include <QDebug>

QAugmentedList::QAugmentedList(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);
    myLayout->setContentsMargins(0,0,0,0);

    myList = new QListWidget();
    myLayout->addWidget(myList);
    QObject::connect(myList, &QListWidget::currentRowChanged, this, &QAugmentedList::OnSelectionChanged);
    QObject::connect(myList, &QListWidget::itemChanged, this, &QAugmentedList::OnListEdited);

    QGridLayout* btnLayout = new QGridLayout();
    myLayout->addLayout(btnLayout);
    btnLayout->setSpacing(3);

#define ADD_BUTTON(row, col, label, callback) { \
    QPushButton* btn = new QPushButton(label); \
    btnLayout->addWidget(btn, row, col); \
    QObject::connect(btn, &QPushButton::clicked, this, &QAugmentedList::callback); \
    btn->setMaximumWidth(25);}

    ADD_BUTTON(0, 0, "<+", OnAddBefore);
    ADD_BUTTON(0, 1, "+>", OnAddAfter);
    ADD_BUTTON(0, 2, "++", OnDuplicate);
    ADD_BUTTON(0, 3, "-", OnRemove);

    ADD_BUTTON(1, 0, "<<", OnMoveFirst);
    ADD_BUTTON(1, 1, "<", OnMoveUp);
    ADD_BUTTON(1, 2, ">", OnMoveDown);
    ADD_BUTTON(1, 3, ">>", OnMoveLast);

#undef ADD_BUTTON
}




int QAugmentedList::Count() const
{
    return myList->count();
}
int QAugmentedList::GetCurrent(QString* _outValue) const
{
    const int currentIndex = myList->currentRow();
    if (_outValue && currentIndex >= 0)
    {
        *_outValue = myList->currentItem()->text();
    }
    return currentIndex;
}
const QString QAugmentedList::GetItemValue(const int _index) const
{
    if (_index < 0 || _index >= myList->count())
        return "";

    return myList->item(_index)->text();
}





#define CHANGE_SIGNAL_ENABLE(enable) myPropagateChanged = enable
void QAugmentedList::Clear()
{
    CHANGE_SIGNAL_ENABLE(false);
    myList->clear();
    CHANGE_SIGNAL_ENABLE(true);
}
void QAugmentedList::AddItemAt(const QString& _label, int _index, const bool _emit)
{
    const int count = myList->count();
    if (_index < 0 || _index > count)
        _index = count;

    CHANGE_SIGNAL_ENABLE(false);

    myList->insertItem(_index, _label);
    auto* item = myList->item(_index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (_emit)
    {
        emit ItemAdded(_index, _label);
        myList->setCurrentRow(_index);
    }
    CHANGE_SIGNAL_ENABLE(true);
}
void QAugmentedList::MoveItemAt(const int _indexFrom, const int _indexTo)
{
    const int count = myList->count();
    if (_indexFrom == _indexTo)
        return;
    if (_indexFrom < 0 || _indexFrom >= count)
        return;
    if (_indexTo < 0 || _indexTo >= count)
        return;

    CHANGE_SIGNAL_ENABLE(false);

    auto* movedItem = myList->takeItem(_indexFrom);
    myList->insertItem(_indexTo, movedItem);
    myList->setCurrentRow(_indexTo);

    emit ItemMoved(_indexFrom, _indexTo);

    CHANGE_SIGNAL_ENABLE(true);
}
void QAugmentedList::RemoveItemAt(const int _index)
{
    if (_index < 0 || _index >= myList->count())
        return;

    CHANGE_SIGNAL_ENABLE(false);

    auto* removedItem = myList->takeItem(_index);
    if (!removedItem)
        return;

    delete removedItem;
    emit ItemRemoved(_index);

    CHANGE_SIGNAL_ENABLE(true);
}

#undef CHANGE_SIGNAL_ENABLE





void QAugmentedList::OnSelectionChanged(const int _index)
{
    emit SelectionChanged(_index);
}
void QAugmentedList::OnListEdited(QListWidgetItem *item)
{
    if (myPropagateChanged)
        emit ItemEdited(myList->row(item), item->text());
}





#define GET_CURRENT const int current = myList->currentRow(); \
                    if (current < 0) \
                        return;
#define GET_CURRENT_OR_ZERO int current = myList->currentRow(); \
                            if (current < 0) { \
                                if (myList->count() == 0) \
                                    current = 0; \
                                else \
                                    return; }\

void QAugmentedList::OnAddBefore()
{
    GET_CURRENT_OR_ZERO;
    AddItemAt("", current, true);
}
void QAugmentedList::OnAddAfter()
{
    GET_CURRENT_OR_ZERO;
    AddItemAt("", current + 1, true);
}
void QAugmentedList::OnDuplicate()
{
    GET_CURRENT;
    AddItemAt(myList->currentItem()->text(), current + 1, true);
}
void QAugmentedList::OnRemove()
{
    GET_CURRENT;
    RemoveItemAt(current);
}

void QAugmentedList::OnMoveFirst()
{
    GET_CURRENT;
    MoveItemAt(current, 0);
}
void QAugmentedList::OnMoveUp()
{
    GET_CURRENT;
    MoveItemAt(current, current - 1);
}
void QAugmentedList::OnMoveDown()
{
    GET_CURRENT;
    MoveItemAt(current, current + 1);
}
void QAugmentedList::OnMoveLast()
{
    GET_CURRENT;
    MoveItemAt(current, myList->count() - 1);
}

#undef GET_CURRENT_OR_ZERO
#undef GET_CURRENT
