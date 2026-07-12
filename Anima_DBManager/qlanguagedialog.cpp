#include "qlanguagedialog.h"

#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "db_manager.h"


QLanguageDialog::QLanguageDialog(QWidget* _parent) :
    QDialog(_parent)
{
    const auto& languageListCopy = DB_Manager::GetDB_Manager().GetLanguages().GetLanguageList();
    for (const auto& l : languageListCopy)
    {
        myLanguageActionList.push_back(LanguageAction(l));
    }

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    auto* pathTitle = new QLabel("Languages :");
    pathTitle->setStyleSheet("font-weight: bold");
    vLayout->addWidget(pathTitle);
    vLayout->addSpacing(3);

    // Languages list
    QHBoxLayout* listLayout = new QHBoxLayout();
    myLanguageListWidget = new QListWidget();
    QObject::connect(myLanguageListWidget, &QListWidget::currentRowChanged, this, &QLanguageDialog::OnSelectionChanged);
    listLayout->addWidget(myLanguageListWidget);
    QVBoxLayout* listActionLayout = new QVBoxLayout();
    myMoveUpBtn = new QPushButton("^");
    myMoveDownBtn = new QPushButton("v");
    myRemoveBtn = new QPushButton("-");
    QObject::connect(myMoveUpBtn, &QPushButton::clicked, this, &QLanguageDialog::OnMoveUp);
    QObject::connect(myMoveDownBtn, &QPushButton::clicked, this, &QLanguageDialog::OnMoveDown);
    QObject::connect(myRemoveBtn, &QPushButton::clicked, this, &QLanguageDialog::OnRemove);
    listActionLayout->addWidget(myMoveUpBtn);
    listActionLayout->addWidget(myRemoveBtn);
    listActionLayout->addWidget(myMoveDownBtn);
    listLayout->addLayout(listActionLayout);
    vLayout->addLayout(listLayout);
    vLayout->addSpacing(3);

    // Language Editor
    QFormLayout* editLayout = new QFormLayout();
    myAbbrevEdit = new QLineEdit();
    myNameEdit = new QLineEdit();
    editLayout->addRow("Code:", myAbbrevEdit);
    editLayout->addRow("Name:", myNameEdit);
    vLayout->addLayout(editLayout);
    vLayout->addSpacing(3);

    // Actions
    QHBoxLayout* actionLayout = new QHBoxLayout();
    myAddBtn = new QPushButton("Add New After");
    myReplaceBtn = new QPushButton("Replace");
    QObject::connect(myAddBtn, &QPushButton::clicked, this, &QLanguageDialog::OnAdd);
    QObject::connect(myReplaceBtn, &QPushButton::clicked, this, &QLanguageDialog::OnReplace);
    actionLayout->addWidget(myAddBtn);
    actionLayout->addWidget(myReplaceBtn);
    vLayout->addLayout(actionLayout);
    vLayout->addSpacing(12);

    // Bttn
    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout();
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QLanguageDialog::OnApply);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    setWindowTitle("Project Languages");

    UpdateListWidget();
    myLanguageListWidget->setCurrentRow(0);
}

const Language& QLanguageDialog::GetFinalLanguageForAction(const LanguageAction& _action)
{
    const QString& originalAbbrev = _action.myLanguageFinalValue.GetAbbrev();
    if (myLanguageRenameActionList.contains(originalAbbrev))
    {
        return myLanguageRenameActionList[originalAbbrev];
    }

    return _action.myLanguageFinalValue;
}
int QLanguageDialog::GetActualLanguageCount() const
{
    int count = 0;
    for (const auto& languageAction : myLanguageActionList)
    {
        if (languageAction.myType == LanguageActionType::REMOVE)
        {
            continue;
        }
        count++;
    }
    return count;
}
QColor QLanguageDialog::GetColorForActionType(LanguageActionType _actionType) const
{
    switch(_actionType)
    {
    default:
    case LanguageActionType::PRISTINE:
        return QColorConstants::White;
    case LanguageActionType::ADD:
        return QColorConstants::Green;
    case LanguageActionType::REMOVE:
        return QColorConstants::Red;
    case LanguageActionType::MOVE:
        return QColorConstants::Yellow;
    }
}
void QLanguageDialog::UpdateListWidget()
{
    myLanguageListWidget->clear();
    static QString textTemplate = QString("[%1] \t%2");
    for (const auto& languageAction : myLanguageActionList)
    {
        const Language& lang = GetFinalLanguageForAction(languageAction);
        QListWidgetItem* languageItem = new QListWidgetItem(textTemplate.arg(lang.GetAbbrev(), lang.GetName()));
        languageItem->setBackground(QBrush{GetColorForActionType(languageAction.myType)});
        myLanguageListWidget->addItem(languageItem);
    }
}

void QLanguageDialog::OnSelectionChanged(const int _index)
{
    bool selectionIsValid = _index >= 0 && _index < myLanguageActionList.size();

    myReplaceBtn->setEnabled(selectionIsValid);
    myRemoveBtn->setEnabled(selectionIsValid && (myLanguageActionList[_index].myType == LanguageActionType::REMOVE || GetActualLanguageCount() > 1));

    if (!selectionIsValid)
    {
        return;
    }

    const Language& lang = GetFinalLanguageForAction(myLanguageActionList[_index]);
    myAbbrevEdit->setText(lang.GetAbbrev());
    myNameEdit->setText(lang.GetName());
}

void QLanguageDialog::OnAdd()
{
    int targetRow = myLanguageListWidget->currentRow() + 1;

    // TODO : check abbrev not blank nor already taken + remove spaces in abbrev and name
    Language newLang {myAbbrevEdit->text(), myNameEdit->text()};
    LanguageAction action {newLang};
    action.myType = LanguageActionType::ADD;
    myLanguageActionList.insert(targetRow, action);

    UpdateListWidget();
    myLanguageListWidget->setCurrentRow(targetRow);
}
void QLanguageDialog::OnReplace()
{
    int targetRow = myLanguageListWidget->currentRow();
    Q_ASSERT(0 <= targetRow && targetRow < myLanguageActionList.size());

    // TODO : check abbrev not blank nor already taken (except by itself) + remove spaces in abbrev and name
    if (myLanguageActionList[targetRow].myType == LanguageActionType::ADD)
    {
        // If add, simply replace what's gonna be added
        myLanguageActionList[targetRow].myLanguageFinalValue = Language {myAbbrevEdit->text(), myNameEdit->text()};
    }
    else
    {
        const QString& originalAbbrev = myLanguageActionList[targetRow].myLanguageFinalValue.GetAbbrev();
        myLanguageRenameActionList.insert(originalAbbrev, Language {myAbbrevEdit->text(), myNameEdit->text()});
    }

    UpdateListWidget();
    myLanguageListWidget->setCurrentRow(targetRow);
}
void QLanguageDialog::OnRemove()
{
    int targetRow = myLanguageListWidget->currentRow();
    Q_ASSERT(0 <= targetRow && targetRow < myLanguageActionList.size());
    Q_ASSERT(myLanguageActionList[targetRow].myType == LanguageActionType::REMOVE || GetActualLanguageCount() > 1);

    const LanguageActionType type = myLanguageActionList[targetRow].myType;
    if (type == LanguageActionType::ADD)
    {
        myLanguageActionList.removeAt(targetRow);
    }
    else if (type == LanguageActionType::REMOVE)
    {
        myLanguageActionList[targetRow].myType = LanguageActionType::PRISTINE;
    }
    else
    {
        myLanguageActionList[targetRow].myType = LanguageActionType::REMOVE;
    }

    UpdateListWidget();
    myLanguageListWidget->setCurrentRow(targetRow < myLanguageActionList.size() ? targetRow : targetRow - 1);
}
void QLanguageDialog::OnMoveUp()
{
    int targetRow = myLanguageListWidget->currentRow();
    Q_ASSERT(0 <= targetRow && targetRow < myLanguageActionList.size());
    if (targetRow == 0)
    {
        return;
    }

    OnMove(targetRow, true);
}
void QLanguageDialog::OnMoveDown()
{
    int targetRow = myLanguageListWidget->currentRow();
    int actionCount = myLanguageActionList.size();
    Q_ASSERT(0 <= targetRow && targetRow < actionCount);
    if (targetRow == actionCount - 1)
    {
        return;
    }

    OnMove(targetRow, false);
}
void QLanguageDialog::OnMove(int _originalRow, bool _up)
{
    int targetRow = _originalRow + (_up ? -1 : 1);
    Q_ASSERT(0 <= targetRow && targetRow < myLanguageActionList.size());

    myLanguageActionList.move(_originalRow, targetRow);
    if (myLanguageActionList[targetRow].myType == LanguageActionType::PRISTINE)
    {
        myLanguageActionList[targetRow].myType = LanguageActionType::MOVE;
    }

    UpdateListWidget();
    myLanguageListWidget->setCurrentRow(targetRow < myLanguageActionList.size() ? targetRow : targetRow - 1);
}


void QLanguageDialog::OnApply()
{
    auto& dbManager = DB_Manager::GetDB_Manager();

    // Apply remove actions and forget related renames
    for (const auto& action : myLanguageActionList)
    {
        if (action.myType != LanguageActionType::REMOVE)
        {
            continue;
        }

        const auto& removedAbbrev = action.myLanguageFinalValue.GetAbbrev();
        myLanguageRenameActionList.remove(removedAbbrev);
        dbManager.RemoveLanguage(removedAbbrev);
    }

    // Compute the desired index for each language
    QMap<QString, int> myDesiredIndexByAbbrev;
    int nextIndex = 0;
    for (const auto& action : myLanguageActionList)
    {
        if (action.myType == LanguageActionType::REMOVE)
        {
            // alreday handled
            continue;
        }

        if(action.myType != LanguageActionType::PRISTINE)
        {
            // for unchanged action, no need to remember the index
            myDesiredIndexByAbbrev.insert(GetFinalLanguageForAction(action).GetAbbrev(), nextIndex);
        }
        nextIndex++;
    }

    // Apply renames
    for (const auto& [originalAbrrev, edited] : myLanguageRenameActionList.asKeyValueRange())
    {
        dbManager.ReplaceLanguageInfo(originalAbrrev, edited);
    }

    // Apply ADD and MOVE actions
    const int actionListCount = myLanguageActionList.size();
    for (const auto& action : myLanguageActionList)
    {
        switch(action.myType)
        {
            default:
            case LanguageActionType::REMOVE:
            case LanguageActionType::PRISTINE:
                continue;
            case LanguageActionType::ADD:
            case LanguageActionType::MOVE:
            {
                const auto& languageInfo = GetFinalLanguageForAction(action);
                const QString& abbrev = languageInfo.GetAbbrev();
                Q_ASSERT(myDesiredIndexByAbbrev.contains(abbrev));
                int targetIndex = myDesiredIndexByAbbrev[abbrev];
                if (action.myType == LanguageActionType::ADD)
                {
                    dbManager.AddLanguage(languageInfo, targetIndex);
                }
                else
                {
                    dbManager.MoveLanguage(abbrev, targetIndex);
                }
            }
        }
    }

    // Check duplicates ? can't revert if wrong so... need to check earlier ?

    QDialog::accept();
}