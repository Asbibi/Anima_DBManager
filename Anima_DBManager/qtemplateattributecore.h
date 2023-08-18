#ifndef QTEMPLATEATTRIBUTECORE_H
#define QTEMPLATEATTRIBUTECORE_H

#include <QWidget>

#include "attributeparam.h"
#include "attribute.h"
#include "qattribute.h"
#include "qtemplatestructurecore.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>

class QTemplateAttributeCore : public QWidget
{
    Q_OBJECT
private:
    TemplateAttribute& myTemplateAttribute;

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
    void OnParamChanged_StructureTemplate(bool _withCriticalChange);

    void OnDefaultAttributeEdited(const QString& _attributeValueAsText);
};

#endif // QTEMPLATEATTRIBUTECORE_H
