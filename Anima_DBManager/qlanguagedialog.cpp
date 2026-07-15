#include "qlanguagedialog.h"

#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>

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
    QObject::connect(myAbbrevEdit, &QLineEdit::editingFinished, this, &QLanguageDialog::OnAbbrevEdited);
    QObject::connect(myNameEdit, &QLineEdit::editingFinished, this, &QLanguageDialog::OnNameEdited);
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

    myEditErrorLabel = new QLabel();
    vLayout->addWidget(myEditErrorLabel);
    myRemoveErrorLabel = new QLabel();
    vLayout->addWidget(myRemoveErrorLabel);

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
    const QString& originalAbbrev = _action.myLanguageValue.GetAbbrev();
    if (myLanguageRenameActionList.contains(originalAbbrev))
    {
        return myLanguageRenameActionList[originalAbbrev];
    }

    return _action.myLanguageValue;
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
        languageItem->setForeground(QBrush{myLanguageRenameActionList.contains(languageAction.myLanguageValue.GetAbbrev()) ?
            QColorConstants::Blue : QColorConstants::Black});
        myLanguageListWidget->addItem(languageItem);
    }
}
LanguageEditCheckResult QLanguageDialog::CanAddOrReplaceWithInputs()
{
    if (myNameEdit->text().isEmpty())
        return LanguageEditCheckResult::NAME_EMPTY;

    const auto& myAbbrev = myAbbrevEdit->text();
    if (myAbbrev.isEmpty())
        return LanguageEditCheckResult::ABBREV_EMPTY;


    bool abbrevDidntChanged = false;
    int i = -1;
    for (const auto& languageAction : myLanguageActionList)
    {
        i++;
        if (languageAction.myType == LanguageActionType::REMOVE)
            continue;

        const QString& abbrev = GetFinalLanguageForAction(languageAction).GetAbbrev();
        if (abbrev == myAbbrev)
        {
            if (i == myLanguageListWidget->currentRow())
            {
                abbrevDidntChanged = true;
                break;
            }

            return LanguageEditCheckResult::ABBREV_ALREADY_USED_OTHER;
        }
    }

    return abbrevDidntChanged ? LanguageEditCheckResult::ABBREV_ALREADY_USED_SELF : LanguageEditCheckResult::OK;
}
void QLanguageDialog::CheckCanAddOrReplaceWithInputs()
{
    LanguageEditCheckResult enabledResult = CanAddOrReplaceWithInputs();
    myAddBtn->setEnabled(enabledResult == LanguageEditCheckResult::OK);
    myReplaceBtn->setEnabled(enabledResult == LanguageEditCheckResult::OK || enabledResult == LanguageEditCheckResult::ABBREV_ALREADY_USED_SELF);

    switch(enabledResult)
    {
        default:
        case LanguageEditCheckResult::OK:
        {
            myEditErrorLabel->setVisible(false);
            break;
        }
        case LanguageEditCheckResult::NAME_EMPTY:
        {
            static const QString addBackErrorText = "<font color='red'>Can't have a language with no Name.</font>";
            myEditErrorLabel->setText(addBackErrorText);
            myEditErrorLabel->setVisible(true);
            break;
        }
        case LanguageEditCheckResult::ABBREV_EMPTY:
        {
            static const QString lastErrorText = "<font color='red'>Can't have a language with no Abbreviation.</font>";
            myEditErrorLabel->setText(lastErrorText);
            myEditErrorLabel->setVisible(true);
            break;
        }
        case LanguageEditCheckResult::ABBREV_ALREADY_USED_OTHER:
        {
            static const QString addBackErrorText = "<font color='red'>This Abbreviation is already used by another language.</font>";
            myEditErrorLabel->setText(addBackErrorText);
            myEditErrorLabel->setVisible(true);
            break;
        }
        case LanguageEditCheckResult::ABBREV_ALREADY_USED_SELF:
        {
            static const QString addBackErrorText = "<font color='gray'>Can't add a new language with the same Abbreviation.</font>";
            myEditErrorLabel->setText(addBackErrorText);
            myEditErrorLabel->setVisible(true);
            break;
        }
    }
}
LanguageRemoveCheckResult QLanguageDialog::ShouldHaveRemoveButton(int _currentRow)
{
    // Invalid index
    if (_currentRow < 0 || myLanguageActionList.size() <= _currentRow)
        return LanguageRemoveCheckResult::INVALID;

    // Case of actually removing the language
    if (myLanguageActionList[_currentRow].myType != LanguageActionType::REMOVE)
        return GetActualLanguageCount() > 1 ? LanguageRemoveCheckResult::OK : LanguageRemoveCheckResult::LAST_LANGUAGE;

    // Case of cancelling the removal of the language : identifier unique ?
    const auto& removedAbbrev = GetFinalLanguageForAction(myLanguageActionList[_currentRow]).GetAbbrev();
    for (const auto& languageAction : myLanguageActionList)
    {
        if (languageAction.myType == LanguageActionType::REMOVE)
            continue;

        const QString& abbrev = GetFinalLanguageForAction(languageAction).GetAbbrev();
        if (abbrev == removedAbbrev)
        {
            return LanguageRemoveCheckResult::ABBREV_ALREADY_USED;
        }
    }
    return LanguageRemoveCheckResult::OK;
}
void QLanguageDialog::CheckRemoveButton(int _currentRow)
{
    LanguageRemoveCheckResult canRemoveResult = ShouldHaveRemoveButton(_currentRow);
    myRemoveBtn->setEnabled(canRemoveResult == LanguageRemoveCheckResult::OK);

    switch(canRemoveResult)
    {
        default:
        case LanguageRemoveCheckResult::OK:
        case LanguageRemoveCheckResult::INVALID:
        {
            myRemoveErrorLabel->setVisible(false);
            break;
        }
        case LanguageRemoveCheckResult::LAST_LANGUAGE:
        {
            static const QString lastErrorText = "<font color='gray'>Can't remove the last language.</font>";
            myRemoveErrorLabel->setText(lastErrorText);
            myRemoveErrorLabel->setVisible(true);
            break;
        }
        case LanguageRemoveCheckResult::ABBREV_ALREADY_USED:
        {
            static const QString addBackErrorText = "<font color='gray'>Can't add back this language : its Abbreviation is already used by another language.</font>";
            myRemoveErrorLabel->setText(addBackErrorText);
            myRemoveErrorLabel->setVisible(true);
            break;
        }
    }
}

void QLanguageDialog::OnSelectionChanged(const int _index)
{
    if (_index < 0 || myLanguageActionList.size() <= _index)
    {
        myReplaceBtn->setEnabled(false);
        myRemoveBtn->setEnabled(false);
        return;
    }

    const Language& lang = GetFinalLanguageForAction(myLanguageActionList[_index]);
    myAbbrevEdit->setText(lang.GetAbbrev());
    myNameEdit->setText(lang.GetName());

    CheckRemoveButton(_index);
    CheckCanAddOrReplaceWithInputs();
}
void QLanguageDialog::OnAbbrevEdited()
{
    QString abbrev = myAbbrevEdit->text();
    abbrev.replace(' ', '_');
    abbrev = abbrev.toUpper();
    static const auto filterRegex = QRegularExpression("[^A-Z0-9_#.-]");
    abbrev.remove(filterRegex);
    myAbbrevEdit->setText(abbrev);

    CheckCanAddOrReplaceWithInputs();
}
void QLanguageDialog::OnNameEdited()
{
    QString name = myNameEdit->text();
    name.replace(' ', '_');
    myNameEdit->setText(name);

    CheckCanAddOrReplaceWithInputs();
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
        myLanguageActionList[targetRow].myLanguageValue = Language {myAbbrevEdit->text(), myNameEdit->text()};
    }
    else
    {
        const QString& originalAbbrev = myLanguageActionList[targetRow].myLanguageValue.GetAbbrev();
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

    // Warn the user if needed
    bool hasLanguageActionImpactingData = false;
    for (const auto& action : myLanguageActionList)
    {
        if (action.myType != LanguageActionType::PRISTINE)
        {
            hasLanguageActionImpactingData = true;
            break;
        }
    }
    if (hasLanguageActionImpactingData)
    {
        auto warnDialogBoxResult = QMessageBox::warning(
            this,
            "Editing Language will impact Data",
            "The changes asked on the project's Languages will impact the String Tables' data.\nContinue ?",
            QMessageBox::StandardButtons(QMessageBox::Ok | QMessageBox::Cancel) );

        if (warnDialogBoxResult != QMessageBox::Ok)
        {
            return;
        }
    }

    // Notify edit starts
    QMessageBox* infoDialogBox = new QMessageBox(
        QMessageBox::Information,
        "Applying changes...",
        "Applying changes to the project's Languages...\nPlease wait until the operation is completed.");
    infoDialogBox->setStandardButtons(QMessageBox::NoButton);
    infoDialogBox->setWindowModality(Qt::ApplicationModal);
    infoDialogBox->setWindowFlags(infoDialogBox->windowFlags() & ~Qt::WindowCloseButtonHint);
    infoDialogBox->setAttribute(Qt::WA_DeleteOnClose);
    infoDialogBox->show();
    QCoreApplication::processEvents();
    dbManager.StartLanguageEditingFromDialogBox();

    // Apply remove actions and forget related renames
    for (const auto& action : myLanguageActionList)
    {
        if (action.myType != LanguageActionType::REMOVE)
        {
            continue;
        }

        const auto& removedAbbrev = action.myLanguageValue.GetAbbrev();
        myLanguageRenameActionList.remove(removedAbbrev);
        dbManager.RemoveLanguage(removedAbbrev);
    }
    QCoreApplication::processEvents();  // Need to call twice so infoDialogBox content is correctly displayed

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
    dbManager.ReplaceLanguageInfos(myLanguageRenameActionList);

    // Apply ADD and MOVE actions
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

    // Notify edit completed
    dbManager.EndLanguageEditingFromDialogBox();
    infoDialogBox->accept();

    QDialog::accept();
}