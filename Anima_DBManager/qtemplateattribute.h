#ifndef QTEMPLATEATTRIBUTE_H
#define QTEMPLATEATTRIBUTE_H

#include <QWidget>

#include "attributeparam.h"
#include "templateattribute.h"
#include "qattribute.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class QTemplateAttribute : public QWidget
{
    Q_OBJECT
private:
    QFormLayout* myFormLayout;
    QLineEdit* myName;
    QString myNameCached;
    QComboBox* myTypeComboBox;

    QPushButton* myApplyBtn;
    QPushButton* myRevertBtn;

    QAttribute* myDefAttribute;
    QPushButton* myResetAllToDefault;
    QLabel* myDefAttributeUnavailable;


    bool myParamHasCriticalChanged;
    AttributeParam myParam;

    void OnParamEdited();
    void ShowDefaultWidgets(bool _show);

public:
    explicit QTemplateAttribute(QWidget* _parent = nullptr);

    void UpdateContent(const TemplateAttribute& _attr);
    void UpdateContent(AttributeTypeHelper::Type _type);

signals:
    void NameChanged(const QString& _previousName, QString& _newName);
    void Applied(const QString& _name, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _hasCriticalChanges);
    void Reverted(const QString& _name);
    void AppliedDefaultToAll(const QString& _name);

public slots:
    void OnParamChanged_Type(const QString& _typeStr);

    void OnParamChanged_IgnoreMin(bool _use);
    void OnParamChanged_IgnoreMax(int _use);
    void OnParamChanged_MinInt(int _min);
    void OnParamChanged_MaxInt(int _max);
    void OnParamChanged_MinFloat(float _min);
    void OnParamChanged_MaxFloat(float _max);
    //void OnParamChanged_AttribTempl(???);
    void OnParamChanged_StructDB(const QString& _SDBName);
    void OnParamChanged_Enum(int _enumIndex);

    void OnNameEdited();
    void OnApply();
    void OnRevert();
    void OnApplyDefaultToAll();
};

#endif // QTEMPLATEATTRIBUTE_H
