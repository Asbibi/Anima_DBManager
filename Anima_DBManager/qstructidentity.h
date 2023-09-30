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
    QLineEdit* myTemplatename;
    QLineEdit* myTemplateAbbrev;
    QComboBox* myIconTypeComboBox;
    QColorEditor* myIconColorEditor;

    void InitIconComboBox();

public:
    explicit QStructIdentity(QWidget *parent = nullptr);

    void SetValueFromTemplate(const TemplateStructure& _template);

signals:

};

#endif // QSTRUCTIDENTITY_H
