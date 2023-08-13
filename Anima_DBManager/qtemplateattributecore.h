#ifndef QTEMPLATEATTRIBUTECORE_H
#define QTEMPLATEATTRIBUTECORE_H

#include <QWidget>

#include "attributeparam.h"
#include "attribute.h"
#include "qattribute.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>

class QTemplateAttributeCore : public QWidget
{
    Q_OBJECT
private:
    AttributeParam& myParam;
    Attribute* myDefAttribute; // Not the owner, it is the TemplateAttribute indirectly represented that is

    QFormLayout* myFormLayout;
    QComboBox* myTypeComboBox;
    QAttribute* myDefAttributeEditor;
    QTemplateAttributeCore* myArrayTemplate = nullptr;
    QLabel* myDefAttributeUnavailable;

    QTemplateAttributeCore(AttributeParam& _param, Attribute* _defAttribute, QWidget* _parent = nullptr);
    void UpdateLayout(AttributeTypeHelper::Type _type);
    void PerformTypeSpecificPreparation(AttributeTypeHelper::Type _type);

public:
    QTemplateAttributeCore(TemplateAttribute& _templateAttribute, QWidget* _parent = nullptr);

    AttributeTypeHelper::Type GetType() const;
    bool HasConfigValid() const;

    void ShowDefaultWidget(bool _show);

signals:
    void ParamEdited(bool _withCriticalChange = false);

public slots:
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

    void OnDefaultAttributeEdited(const QString& _attributeValueAsText);
};

#endif // QTEMPLATEATTRIBUTECORE_H
