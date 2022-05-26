#include "mainwindow.h"

//#include<QDebug>
//#include "structure.h"
//#include "astring.h"
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "aarray.h"
#include "abool.h"
#include "aenumerator.h"
#include "aint.h"
#include "afloat.h"
#include "astring.h"
#include "astructure.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      myManager(DB_Manager::GetDB_Manager())
{
    QWidget* wid = new QWidget();
    setCentralWidget(wid);
    //QMainWindow::centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* layout = new QHBoxLayout(wid);

    myTabPanel = new QTabWidget();
    myAttr = new QAttribute(myTabPanel);
    myTabPanel->addTab(myAttr, "Attribute");
    myTabPanel->addTab(new QTextEdit(myTabPanel), "Edit Text");
    layout->addWidget(myTabPanel);
    //myTabPanel->resize(570,420);
    //resize(570,420);
    //QGroupBox for grouping things maybe for creating structures templates or enum


    AttributeParam* someParam = new AttributeParam();

    ABool* boolAttr_true = new ABool(true);
    ABool* boolAttr_false = new ABool(false);
    AInt* intAttr_0 = new AInt(someParam, 0);
    AInt* intAttr_10 = new AInt(someParam, 10);
    AFloat* floatAttr_0 = new AFloat(someParam, 0.5f);
    AFloat* floatAttr_1 = new AFloat(someParam, 0.6654845f);
    AString* stringAttr_hey = new AString(someParam, "Hey");
    AString* stringAttr_hw = new AString(someParam, "Hello world !!");
    Enumerator* enumm = new Enumerator("Enum", {"Fire","Earth","Wind"});
    //AEnumerator* enumAttr_f = new AEnumerator(enumm, 0);
    AEnumerator* enumAttr_w = new AEnumerator(enumm, 2);

    myDebugAttributes.push_back(boolAttr_true);
    myDebugAttributes.push_back(boolAttr_false);
    myDebugAttributes.push_back(intAttr_0);
    myDebugAttributes.push_back(intAttr_10);
    myDebugAttributes.push_back(floatAttr_0);
    myDebugAttributes.push_back(floatAttr_1);
    myDebugAttributes.push_back(stringAttr_hey);
    myDebugAttributes.push_back(stringAttr_hw);
    //myDebugAttributes.push_back(enumAttr_f);
    myDebugAttributes.push_back(enumAttr_w);

    QGroupBox* group = new QGroupBox("Attributes");
    QVBoxLayout *vbox = new QVBoxLayout(group);

    for (const auto* attr : myDebugAttributes)
    {
        QPushButton* btn = new QPushButton(attr->GetDisplayedText());
        QObject::connect(btn, &QPushButton::clicked, [this, attr](){
            if (attr != nullptr)
                myAttr->UpdateAttribute(attr);
        });
        vbox->addWidget(btn);
    }

    //vbox->addStretch(1);
    group->setLayout(vbox);
    layout->addWidget(group);
}

MainWindow::~MainWindow()
{
    for (auto* attr : myDebugAttributes)
    {
        delete attr;
    }
    if (myAttr != nullptr)
        delete myAttr;
    if (myTabPanel != nullptr)
        delete myTabPanel;
}

