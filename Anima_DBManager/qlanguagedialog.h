#ifndef QLANGUAGEDIALOG_H
#define QLANGUAGEDIALOG_H

#include <QDialog>

#include <QList>
#include <QMap>
#include <QListWidget>
#include <QLineEdit>
#include "languageaction.h"

enum class LanguageEditCheckResult
{
    OK,

    NAME_EMPTY,
    ABBREV_EMPTY,
    ABBREV_ALREADY_USED_OTHER,
    ABBREV_ALREADY_USED_SELF
};
enum class LanguageRemoveCheckResult
{
    OK,

    INVALID,
    LAST_LANGUAGE,
    ABBREV_ALREADY_USED
};

class QLanguageDialog : public QDialog
{
    Q_OBJECT

private:
    QList<LanguageAction> myLanguageActionList;
    QMap<QString, Language> myLanguageRenameActionList;

    QListWidget* myLanguageListWidget;
    QLineEdit* myAbbrevEdit;
    QLineEdit* myNameEdit;
    QPushButton* myAddBtn;
    QPushButton* myReplaceBtn;
    QPushButton* myRemoveBtn;
    QPushButton* myMoveUpBtn;
    QPushButton* myMoveDownBtn;

    const Language& GetFinalLanguageForAction(const LanguageAction& _action);
    int GetActualLanguageCount() const;
    QColor GetColorForActionType(LanguageActionType _actionType) const;
    void UpdateListWidget();
    LanguageEditCheckResult CanAddOrReplaceWithInputs();
    void CheckCanAddOrReplaceWithInputs();
    LanguageRemoveCheckResult ShouldHaveRemoveButton(int _currentRow);
    void CheckRemoveButton(int _currentRow);

    void OnMove(int _originalRow, bool _up);

public:
    QLanguageDialog(QWidget* _parent = nullptr);

public slots:
    void OnSelectionChanged(const int _index);
    void OnAbbrevEdited();
    void OnNameEdited();

    void OnAdd();
    void OnReplace();
    void OnRemove();
    void OnMoveUp();
    void OnMoveDown();

    void OnApply();
};

#endif // QLANGUAGEDIALOG_H
