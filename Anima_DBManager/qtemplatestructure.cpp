#include "qtemplatestructure.h"

#include "db_manager.h"

#include <QGridLayout>
#include <QVBoxLayout>

QTemplateStructure::QTemplateStructure(QWidget *parent)
    : QWidget{parent},
      myStructureDB{nullptr}
{
    QVBoxLayout* myLayout = new QVBoxLayout();
    setLayout(myLayout);

    myTabWidget = new QTabWidget();
    myLayout->addWidget(myTabWidget);
    //myTabWidget->setStyleSheet("QTabBar::tab { height: 100px; width: 100px; }");
    myTabWidget->setStyleSheet("QTabBar::tab { height: 40px; }");
    myTabWidget->setMovable(true);
    QTabBar* mTabBar = myTabWidget->tabBar();
    QObject::connect(mTabBar, &QTabBar::tabMoved, this, &QTemplateStructure::OnTabMoved);

    QGridLayout* myButtonLayout = new QGridLayout();
    myLayout->addLayout(myButtonLayout);
    myButtonLayout->setSpacing(3);

#define ADD_BUTTON(row, col, label, callback) { \
    QPushButton* btn = new QPushButton(label); \
    myButtonLayout->addWidget(btn, row, col); \
    QObject::connect(btn, &QPushButton::clicked, this, &QTemplateStructure::callback); \
    btn->setMaximumWidth(25); \
    myPushBtns.append(btn);}

    ADD_BUTTON(0, 0, "<+", OnAddBefore);
    ADD_BUTTON(0, 1, "+>", OnAddAfter);
    ADD_BUTTON(0, 2, "++", OnDuplicate);
    ADD_BUTTON(0, 3, "-", OnRemove);

#undef ADD_BUTTON

    UpdateContent();
}


void QTemplateStructure::UpdateAttributeTabText(int index)
{
    if (!myStructureDB)
        return;

    const auto* attrTmpl = myStructureDB->GetTemplate().GetAttributeTemplate(index);
    if (!attrTmpl)
    {
        qCritical("No template attribute at this index");
        return;
    }

    const QString tabNameBase = "%1\n[%2]";
    myTabWidget->setTabText(index, tabNameBase.arg(attrTmpl->GetName(), AttributeTypeHelper::TypeToString(attrTmpl->GetType())));
    Q_ASSERT(attrTmpl->GetName() == myAttributeNames[index]);
}

void QTemplateStructure::SetStructureDB(StructureDB* _structureDB)
{
    if (myStructureDB == _structureDB)
        return;

    myStructureDB = _structureDB;
    UpdateContent();
}

void QTemplateStructure::UpdateContent()
{
    myAttributeNames.clear();
    myTabWidget->clear();
    bool hasStructPtr = myStructureDB;
    myTabWidget->setEnabled(hasStructPtr);
    for (auto* btn : myPushBtns)
    {
        btn->setEnabled(hasStructPtr);
    }

    if (!hasStructPtr)
    {
        return;
    }

    const QString tabNameBase = "%1\n[%2]";
    for (const auto* attr : myStructureDB->GetTemplate().GetAttributes())
    {
        QTemplateAttribute* qattr = new QTemplateAttribute();
        myAttributeNames.push_back(attr->GetName());
        qattr->UpdateContent(*attr);
        myTabWidget->addTab(qattr, tabNameBase.arg(attr->GetName(), AttributeTypeHelper::TypeToString(attr->GetType())));
        QObject::connect(qattr, &QTemplateAttribute::NameChanged, this, &QTemplateStructure::OnNameChanged);
        QObject::connect(qattr, &QTemplateAttribute::Applied, this, &QTemplateStructure::OnApply);
        QObject::connect(qattr, &QTemplateAttribute::Reverted, this, &QTemplateStructure::OnRevert);
        QObject::connect(qattr, &QTemplateAttribute::AppliedDefaultToAll, this, &QTemplateStructure::OnApplyDefaultToAll);
    }
}



void QTemplateStructure::OnTabMoved(int _indexFrom, int _indexTo)
{
    auto item = myAttributeNames.takeAt(_indexFrom);
    myAttributeNames.insert(_indexTo, item);
    DB_Manager::GetDB_Manager().MoveStructureAttribute(myStructureDB->GetTemplateName(), _indexFrom, _indexTo);
}
void QTemplateStructure::OnNameChanged(const QString& _previousName, QString& _newName)
{
    int index = myAttributeNames.indexOf(_previousName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().RenameStructureAttribute(myStructureDB->GetTemplateName(), index, _newName);
    myAttributeNames[index] = _newName;
    UpdateAttributeTabText(index);
}
void QTemplateStructure::OnApply(const QString& _attrName, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _hasCriticalChanges)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().ChangeAttributeTemplate(myStructureDB->GetTemplateName(), index, _newType, _param, _hasCriticalChanges);
    OnRevert(_attrName);

    if (_hasCriticalChanges)
        UpdateAttributeTabText(index);
}
void QTemplateStructure::OnRevert(const QString& _attrName)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    QTemplateAttribute* qattr = dynamic_cast<QTemplateAttribute*>(myTabWidget->widget(index));
    if (!qattr)
        return;

    qattr->UpdateContent(*(myStructureDB->GetTemplate().GetAttributeTemplate(index)));
}
void QTemplateStructure::OnApplyDefaultToAll(const QString& _attrName)
{
    int index = myAttributeNames.indexOf(_attrName);
    if (index < 0)
        return;

    DB_Manager::GetDB_Manager().ResetAttributesToDefaultValue(myStructureDB->GetTemplateName(), index);
}



void QTemplateStructure::AddAttribute(int _position, bool _duplicatePrevious)
{
    DB_Manager::GetDB_Manager().AddAttributeTemplate(myStructureDB->GetTemplateName(), _position, _duplicatePrevious);
    UpdateContent();
    myTabWidget->setCurrentIndex(_position);
}
void QTemplateStructure::OnAddBefore()
{
    int position = myTabWidget->currentIndex();
    if (position < 0)
        position = 0;
    AddAttribute(position);
}
void QTemplateStructure::OnAddAfter()
{
    int position = myTabWidget->currentIndex() + 1;
    AddAttribute(position);
}
void QTemplateStructure::OnDuplicate()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    AddAttribute(current + 1, true);
}
void QTemplateStructure::OnRemove()
{
    int current = myTabWidget->currentIndex();
    if (current < 0)
        return;

    DB_Manager::GetDB_Manager().RemoveAttributeTemplate(myStructureDB->GetTemplateName(), current);
    UpdateContent();
    myTabWidget->setCurrentIndex(current);
}
