#ifndef QTEMPLATEATTRIBUTE_H
#define QTEMPLATEATTRIBUTE_H

#include <QWidget>

#include "templateattribute.h"
#include "qtemplateattributecore.h"

#include <QLineEdit>
#include <QPushButton>

class QTemplateAttribute : public QWidget
{
    Q_OBJECT
private:
    TemplateAttribute* myTemplateCopy;

    QFormLayout* myFormLayout;
    QLineEdit* myName;
    QString myNameCached;
    QTemplateAttributeCore* myCoreEditor;

    QPushButton* myApplyBtn;
    QPushButton* myRevertBtn;
    QPushButton* myResetAllToDefault;

    bool myCriticalChanges;
    bool myInvalidChanges;

    void ShowDefaultWidget(bool _show);
    void DeleteTemplateIfExists();

public:
    explicit QTemplateAttribute(QWidget* _parent = nullptr);
    ~QTemplateAttribute();

    void UpdateTemplateAttribute(const TemplateAttribute* _attr);


signals:
    void NameChanged(const QString& _previousName, QString& _newName);
    void Applied(const QString& _name, const TemplateAttribute& _editedTemplateCopy, bool _hasCriticalChanges);
    void Reverted(const QString& _name);
    void AppliedDefaultToAll(const QString& _name);

public slots:
    void OnParamEdited(bool _withCriticalChange = false);

    void OnNameEdited();
    void OnApply();
    void OnRevert();
    void OnApplyDefaultToAll();
};

#endif // QTEMPLATEATTRIBUTE_H
