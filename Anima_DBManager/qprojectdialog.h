#ifndef QPROJECTDIALOG_H
#define QPROJECTDIALOG_H

#include <QDialog>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>


class QProjectDialog : public QDialog
{
    Q_OBJECT

private:
    static const QString ourOriginalAttributeName;
    static const QString ourContentColor;
    static const QString ourPrefixColor;
    static const QString ourSuffixColor;

    QString mySaveFilePath;
    QString myProjectPathText;
    QLabel* myProjectPath;
    QLabel* myRelativeProjectPath;
    QCheckBox* myProjectPathIsRelative;

    QTableWidget* myUAssetRegex;
    QLineEdit* myPrefixEdit;
    QLineEdit* mySuffixEdit;
    QLabel* myFixResult;

    QCheckBox* myAutoSaveEnable;
    QSpinBox* myAutoSaveInterval;

    void InitPath(const QString& _path);
    void SetPath(const QString& _path);
    void OnFixChanged(bool _isPrefix);
    void UpdateFixResult();

public:
    QProjectDialog(QWidget* _parent = nullptr);

public slots:
    void OnRelativePathToggle();
    void OnSelectPath();
    void OnResetPath();

    void OnPrefixChange();
    void OnSuffixChange();

    void OnApplyBtnClicked();
};

#endif // QPROJECTDIALOG_H
