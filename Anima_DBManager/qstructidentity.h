#ifndef QSTRUCTIDENTITY_H
#define QSTRUCTIDENTITY_H

#include <QWidget>

#include "templatestructure.h"
#include "qcoloreditor.h"
#include <QLineEdit>
#include <QComboBox>

class QStructIdentity : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* myTemplateName;
    QLineEdit* myTemplateAbbrev;
    QComboBox* myIconTypeComboBox;
    QColorEditor* myIconColorEditor;

    void InitIconComboBox();

public:
    explicit QStructIdentity(QWidget *parent = nullptr);

    void SetValueFromTemplate(const TemplateStructure& _template);

signals:
    void NameChanged(const QString& _name);
    void AbbrevChanged(const QString& _abbrev);
    void IconChanged(const IconManager::IconType _iconType);
    void ColorChanged(const QColor& _color);

public slots:
    void OnNameChanged();
    void OnAbbrevChanged();
    void OnIconChanged(const int _iconTypeIndex);
    //void OnColorChanged(const QColor& _color);

};

#endif // QSTRUCTIDENTITY_H
