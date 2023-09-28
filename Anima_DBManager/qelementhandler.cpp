#include "qelementhandler.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>

QElementHandler::QElementHandler(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    myLayout->setContentsMargins(0,0,0,0);
    setLayout(myLayout);

    QHBoxLayout* selectedLayout = new QHBoxLayout();
    selectedLayout->setContentsMargins(0,0,0,0);
    myCurrentLabel = new QLabel();
    selectedLayout->addWidget(myCurrentLabel);
    QSpinBox* selector = new QSpinBox();
    selector->setMinimum(-1);
    selector->setValue(-1);
    QObject::connect(selector, &QSpinBox::valueChanged, this, &QElementHandler::SpinBoxSelected);
    selectedLayout->addWidget(selector);

    myCurrentTextPreview = new QLabel();
    myCurrentTextPreview->setStyleSheet("font: italic; color: darkgray");

    QGridLayout* btnLayout = new QGridLayout();
    btnLayout->setSpacing(3);
#define ADD_BUTTON(row, col, label, callback) { \
    QPushButton* btn = new QPushButton(label); \
    btnLayout->addWidget(btn, row, col); \
    QObject::connect(btn, &QPushButton::clicked, this, &QElementHandler::callback); \
    btn->setMaximumWidth(25); \
    myButtons.append(btn);}

    ADD_BUTTON(0, 0, "<+", OnAddBefore);
    ADD_BUTTON(0, 1, "+>", OnAddAfter);
    ADD_BUTTON(0, 2, "++", OnDuplicate);
    ADD_BUTTON(0, 3, "-", OnRemove);

    ADD_BUTTON(1, 0, "<<", OnMoveFirst);
    ADD_BUTTON(1, 1, "<", OnMoveUp);
    ADD_BUTTON(1, 2, ">", OnMoveDown);
    ADD_BUTTON(1, 3, ">>", OnMoveLast);
#undef ADD_BUTTON


    myLayout->addLayout(selectedLayout);
    myLayout->addWidget(myCurrentTextPreview);
    myLayout->addLayout(btnLayout);
    OnSelectElement(-1, "");
}

void QElementHandler::OnSelectElement(int _index, const QString& _content)
{
    bool indexValid = _index >= 0;
    for (auto* btn : myButtons)
        btn->setEnabled(indexValid);

    if (indexValid)
    {
        myCurrentLabel->setText(QString{"<font color=\"darkblue\">Current Item: <b>%1</b></font>"}.arg(_index));
        myCurrentTextPreview->setText(_content);
        myCurrentTextPreview->show();
        myElementIndex = _index;
    }
    else
    {
         myCurrentLabel->setText("---");
         myCurrentTextPreview->hide();
         myElementIndex = -1;
    }
}


void QElementHandler::OnAddBefore()
{
    emit AddRequested(myElementIndex);
}
void QElementHandler::OnAddAfter()
{
    emit AddRequested(myElementIndex + 1);
}
void QElementHandler::OnDuplicate()
{
    emit DuplicateRequested(myElementIndex + 1, myElementIndex);
}
void QElementHandler::OnRemove()
{
    emit RemoveRequested(myElementIndex);
}


void QElementHandler::OnMoveFirst()
{
    if (myElementIndex > 0)
        emit MoveRequested(myElementIndex, 0);
}
void QElementHandler::OnMoveUp()
{
    if (myElementIndex > 0)
        emit MoveRequested(myElementIndex, myElementIndex - 1);
}
void QElementHandler::OnMoveDown()
{
    emit MoveRequested(myElementIndex, myElementIndex + 1);
}
void QElementHandler::OnMoveLast()
{
    emit MoveRequested(myElementIndex, -1);
}

