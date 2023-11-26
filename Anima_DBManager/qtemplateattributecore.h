#ifndef QTEMPLATEATTRIBUTECORE_H
#define QTEMPLATEATTRIBUTECORE_H

#include <QWidget>

#include "attributeparam.h"
#include "attribute.h"
#include "qattribute.h"
#include "qtemplatestructurecore.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

class QTemplateAttributeCore : public QWidget
{
    Q_OBJECT
private:
    TemplateAttribute& myTemplateAttribute;

    QLineEdit* myName = nullptr;
    QCheckBox* myActiveCheckBox = nullptr;
    QFormLayout* myFormLayout;
    QComboBox* myTypeComboBox;
    QAttribute* myDefAttributeEditor;
    QTemplateAttributeCore* myArrayTemplate = nullptr;
    QTemplateStructureCore* myStructureTemplate = nullptr;
    QLabel* myDefAttributeUnavailable;

    void UpdateLayout(AttributeTypeHelper::Type _type);
    void PerformTypeSpecificPreparation(AttributeTypeHelper::Type _type);
    void RefreshDefaultAttributeWidget();
    void ReConstructDefaultAttribute(AttributeTypeHelper::Type _type);

public:
    QTemplateAttributeCore(TemplateAttribute& _templateAttribute, bool _withNameField = false, bool _withActive = true, QWidget* _parent = nullptr);

    AttributeTypeHelper::Type GetType() const;
    bool HasConfigValid() const;

    void ShowDefaultWidget(bool _show);

signals:
    void ParamEdited(bool _withCriticalChange = false);
    void NameChanged(QString& _name);
    void TypeChanged(const AttributeTypeHelper::Type _type);

public slots:
    void OnParamChanged_Name();
    void OnParamChanged_Active(int _state);
    void OnParamChanged_Type(const QString& _typeStr);

    void OnParamChanged_IgnoreMin(bool _use);
    void OnParamChanged_IgnoreMax(int _use);
    void OnParamChanged_MinInt(int _min);
    void OnParamChanged_MaxInt(int _max);
    void OnParamChanged_MinFloat(float _min);
    void OnParamChanged_MaxFloat(float _max);
    void OnParamChanged_StructDB(const QString& _SDBName);
    void OnParamChanged_Enum(int _enumIndex);
    void OnParamChanged_ArrayTemplate(bool _withCriticalChange);
    void OnParamChanged_StructureTemplate(bool _withCriticalChange);

    void OnDefaultAttributeEdited(const QJsonValue& _attributeValue);
    void OnResetAStructureDefault();
};

#endif // QTEMPLATEATTRIBUTECORE_H
