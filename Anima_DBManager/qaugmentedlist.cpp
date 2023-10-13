#include "qaugmentedlist.h"

#include "sstringhelper.h"

#include <QGridLayout>
#include <QVBoxLayout>

#include <QDebug>

QAugmentedList::QAugmentedList(bool _unique, const QString& _defValue, QWidget *parent) :
    QWidget{parent},
    myOnlyUniqueValues {_unique},
    myDefaultValue{_defValue}
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

#define ADD_BUTTON_NO_BRACES(row, col, label, callback) \
    QPushButton* btn = new QPushButton(label); \
    btnLayout->addWidget(btn, row, col); \
    QObject::connect(btn, &QPushButton::clicked, this, &QAugmentedList::callback); \
    btn->setMaximumWidth(25);
#define ADD_ADD_BUTTON(row, col, label, callback) { ADD_BUTTON_NO_BRACES(row, col, label, callback)\
    myAddButtons.push_back(btn);}
#define ADD_BUTTON(row, col, label, callback) { ADD_BUTTON_NO_BRACES(row, col, label, callback) }

    ADD_ADD_BUTTON(0, 0, "<+", OnAddBefore);
    ADD_ADD_BUTTON(0, 1, "+>", OnAddAfter);
    ADD_ADD_BUTTON(0, 2, "++", OnDuplicate);
    {
        myRemoveButton = new QPushButton("-");
        btnLayout->addWidget(myRemoveButton, 0, 3);
        QObject::connect(myRemoveButton, &QPushButton::clicked, this, &QAugmentedList::OnRemove);
        myRemoveButton->setMaximumWidth(25);
    }

    ADD_BUTTON(1, 0, "<<", OnMoveFirst);
    ADD_BUTTON(1, 1, "<", OnMoveUp);
    ADD_BUTTON(1, 2, ">", OnMoveDown);
    ADD_BUTTON(1, 3, ">>", OnMoveLast);

#undef ADD_BUTTON
}

void QAugmentedList::CheckMinMax()
{
    const int count = Count();
    const int actualMin = myUseMin ? myMin : (myDisableRemoveLast ? 1 : 0);
    const bool canRemove = count > actualMin;
    const bool canAdd = !(myUseMax && count >= myMax);
    myRemoveButton->setEnabled(canRemove);
    for (auto* btn : myAddButtons)
    {
        btn->setEnabled(canAdd);
    }
}




void QAugmentedList::SetItemEditable(bool _editable)
{
    if (_editable)
        myList->setEditTriggers(QAbstractItemView::DoubleClicked);
    else
        myList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void QAugmentedList::SetMinMax(bool _useMin, int _min, bool _useMax, int _max)
{
    myUseMin = _useMin;
    myMin = _min;
    myUseMax = _useMax;
    myMax = _max;
    CheckMinMax();
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
void QAugmentedList::SetItemValue(const int _index, const QString& _value, bool _emitEditedSignal)
{
    if (_index < 0 || _index >= myList->count())
        return;

    myList->item(_index)->setText(_value);
    if (_emitEditedSignal)
        emit ItemEdited(_index, _value);
}


void QAugmentedList::SelectItemAt(const int _index)
{
    if (_index < 0 || _index >= myList->count())
        myList->setCurrentRow(-1);
    else
        myList->setCurrentRow(_index);
}
void QAugmentedList::SelectItem(const QString& _text)
{
    const int count = myList->count();
    for (int i = 0; i < count; i++)
    {
        if (myList->item(i)->text() == _text)
        {
            SelectItemAt(i);
            return;
        }
    }
}




#define CHANGE_SIGNAL_ENABLE(enable) myPropagateChanged = enable
void QAugmentedList::Clear()
{
    CHANGE_SIGNAL_ENABLE(false);
    myList->clear();
    CHANGE_SIGNAL_ENABLE(true);
}
void QAugmentedList::AddItemAt(const QString& _label, int _index, const bool _emit, const int _copyFrom)
{
    const int count = myList->count();
    if (_index < 0 || _index > count)
        _index = count;

    CHANGE_SIGNAL_ENABLE(false);

    QString identifier = _label;
    if (myOnlyUniqueValues)
    {
        QString baseIdentifier = _label;
        auto validate = [this](const QString& _identifier)->bool{
            const int count = myList->count();
            for (int i = 0; i < count; i++)
            {
                if (myList->item(i)->text() == _identifier)
                    return false;
            }
            return true;
        };
        identifier = SStringHelper::GetUniqueIdentifier(baseIdentifier, validate, true);
    }

    myList->insertItem(_index, identifier);
    auto* item = myList->item(_index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (_emit)
    {
        if (_copyFrom < 0)
            emit ItemAdded(_index, identifier);
        else
            emit ItemDuplicated(_index, _copyFrom);
        myList->setCurrentRow(_index);
    }

    if (myDisableRemoveLast && myList->count() > 1)
        myRemoveButton->setEnabled(true);

    CHANGE_SIGNAL_ENABLE(true);

    CheckMinMax();
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

    bool canRemove = true;
    emit ItemRemoveCheck(_index, canRemove);
    if (!canRemove)
        return;

    CHANGE_SIGNAL_ENABLE(false);

    auto* removedItem = myList->takeItem(_index);
    if (!removedItem)
        return;

    delete removedItem;
    emit ItemRemoved(_index);

    if (myDisableRemoveLast && myList->count() <= 1)
        myRemoveButton->setEnabled(false);

    CHANGE_SIGNAL_ENABLE(true);

    CheckMinMax();
}

#undef CHANGE_SIGNAL_ENABLE





void QAugmentedList::OnSelectionChanged(const int _index)
{
    emit SelectionChanged(_index);
}
void QAugmentedList::OnListEdited(QListWidgetItem *item)
{
    if (!myPropagateChanged)
        return;

    const int editedRow = myList->row(item);
    QString value = item->text();

    if (myOnlyUniqueValues)
    {
        QString wantedValue = value;
        auto validate = [this, &editedRow](const QString& _identifier)->bool{
            const int count = myList->count();
            for (int i = 0; i < count; i++)
            {
                if (myList->item(i)->text() == _identifier && i != editedRow)
                    return false;
            }
            return true;
        };
        value = SStringHelper::GetUniqueIdentifier(wantedValue, validate, true);
    }

    if (item->text() != value)
        item->setText(value);
    else
        emit ItemEdited(editedRow, value);
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
    AddItemAt(myDefaultValue, current, true);
}
void QAugmentedList::OnAddAfter()
{
    GET_CURRENT_OR_ZERO;
    AddItemAt(myDefaultValue, current + 1, true);
}
void QAugmentedList::OnDuplicate()
{
    GET_CURRENT;
    AddItemAt(myList->currentItem()->text(), current + 1, true, current);
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
