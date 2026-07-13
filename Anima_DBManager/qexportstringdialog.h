#ifndef QEXPORTSTRINGDIALOG_H
#define QEXPORTSTRINGDIALOG_H

#include <QDialog>
#include <QSet>
#include <QLabel>
#include <QLineEdit>
#include "qexportstringconflictpolicy.h"


enum class ExportError
{
    OK,

    NOTHING_TO_EXPORT,
    NO_EXPORT_NAME,
    NO_EXPORT_FOLDER,
    NO_LANGUAGE_DIFF,
    NO_TABLE_DIFF
};


class QExportStringDialog : public QDialog
{
    Q_OBJECT

private:
    bool myUserSelectTables;
    QSet<int> myTablesToExportList;
    QSet<int> myLanguageToExportList;
    QString myExportFolder;
    QExportStringConflictPolicy myConflictPolicy = QExportStringConflictPolicy::OVERWRITE;

    QLabel* myExportFolderText;
    QLineEdit* myExportNameTemplateText;
    QPushButton* myApplyBtn;
    QLabel* myExportErrorLabel;

    void InitWidget();
    void SetDirPath(const QString& _path);
    ExportError CanApply() const;

public:
    QExportStringDialog(bool _userSelectTables, QWidget* _parent = nullptr);
    QExportStringDialog(int _tableToImport, QWidget* _parent = nullptr);

    const QSet<int>& GetTablesToExport() const;
    const QSet<int>& GetLanguagesToExport() const;
    const QString& GetExportFolderPath() const;
    QString GetExportFileNameTemplate() const;
    QExportStringConflictPolicy GetConflictPolicy() const;

public slots:
    void UpdateApplyButtonEnable();

    void OnApplyBtnClicked();
    void OnLanguageChecked(Qt::CheckState _state, int _languageIndex);
    void OnTableChecked(Qt::CheckState _state, int _tableIndex);
    void OnDirBtnClicked();
    void OnConflictPolicyChanged(int _policyIndex);
};

#endif // QEXPORTSTRINGDIALOG_H
