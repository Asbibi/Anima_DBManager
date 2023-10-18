#include "qtemplatestructurebase.h"

#include <QGridLayout>
#include <QTabBar>
#include <QVBoxLayout>

QtemplateStructureBase::QtemplateStructureBase(QWidget* _parent)
    : QWidget{_parent}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);

    myTabWidget = new QTabWidget();
    myLayout->addWidget(myTabWidget);
    myTabWidget->setStyleSheet("QTabBar::tab { height: 40px; }");
    myTabWidget->setMovable(true);
    QTabBar* mTabBar = myTabWidget->tabBar();
    QObject::connect(mTabBar, &QTabBar::tabMoved, this, &QtemplateStructureBase::OnTabMoved);

    QGridLayout* myButtonLayout = new QGridLayout();
    myLayout->addLayout(myButtonLayout);
    myButtonLayout->setSpacing(3);

#define ADD_BUTTON(row, col, label, callback) { \
    QPushButton* btn = new QPushButton(label); \
    myButtonLayout->addWidget(btn, row, col); \
    QObject::connect(btn, &QPushButton::clicked, this, &QtemplateStructureBase::callback); \
    btn->setMaximumWidth(25); \
    myPushBtns.append(btn);}

    ADD_BUTTON(0, 0, "<+", OnAddBefore);
    ADD_BUTTON(0, 1, "+>", OnAddAfter);
    ADD_BUTTON(0, 2, "++", OnDuplicate);
    ADD_BUTTON(0, 3, "-", OnRemove);

#undef ADD_BUTTON
}
