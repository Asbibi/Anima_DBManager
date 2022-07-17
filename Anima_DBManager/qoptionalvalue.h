#ifndef QOPTIONALVALUE_H
#define QOPTIONALVALUE_H

#include <QWidget>

#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

class QOptionalValue_Int : public QWidget
{
    Q_OBJECT
private:
    QCheckBox* myCheckBox;
    QSpinBox* mySpinBox;

public:
    explicit QOptionalValue_Int(QWidget *parent = nullptr);
    void SetValues(bool _enabled, int _value);
    void SetMinimum(int _value);

signals:
    void OnEnableChanged(bool _enabled);
    void OnValueChanged(int _value);

public slots:
    void MyCheckBoxChanged(int _state);
    void MyValueChanged(int _value);
};



class QOptionalValue_Float : public QWidget
{
    Q_OBJECT
private:
    QCheckBox* myCheckBox;
    QDoubleSpinBox* mySpinBox;

public:
    explicit QOptionalValue_Float(QWidget *parent = nullptr);
    void SetValues(bool _enabled, float _value);
    void SetMinimum(float _value);

signals:
    void OnEnableChanged(bool _enabled);
    void OnValueChanged(float _value);

public slots:
    void MyCheckBoxChanged(int _state);
    void MyValueChanged(float _value);
};


#endif // QOPTIONALVALUE_H
