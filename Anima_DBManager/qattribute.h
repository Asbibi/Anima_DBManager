#ifndef QATTRIBUTE_H
#define QATTRIBUTE_H

#include <QWidget>

#include "attribute.h"

#include <QPushButton>



class QAttribute : public QWidget
{
    Q_OBJECT

private:
    Attribute::Type myType = Attribute::Type::Invalid;
    const void* myAttributePtr = nullptr;   // Only use adress

    QWidget* myContent = nullptr;
    QPushButton* myEditButton = nullptr;

    void LogErrorCast() const;
    void DeleteContent();
    void RebuildAttributeWidget(const Attribute* _attribute);
    void RebuildWidgetFromType(const Attribute::Type _type);
    void BuildMoreButton();

public:
    explicit QAttribute(QWidget* parent = nullptr);
    ~QAttribute();

signals:
    void OnWidgetValueChanged(const QString& _thisQAttribute);    // arg given during emit()

public slots:
    void UpdateAttribute(const Attribute* _attribute);
    void ContentStateChanged();//Attribute& _attribute);
};

#endif // QATTRIBUTE_H
