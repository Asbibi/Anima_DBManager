#ifndef QCOLOREDITOR_H
#define QCOLOREDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QSlider>

class QColorEditor : public QWidget
{
    Q_OBJECT

private:
    QColor myColor;

    QLineEdit* myColorText;
    QSlider* myHueSlider;
    QSlider* mySaturationSlider;
    QSlider* myValueSlider;

    void UpdateColorText();

public:
    explicit QColorEditor(QWidget *parent = nullptr);
    void SetColor(const QColor& _color);
    const QColor& GetColor() const;

signals:
    void ColorChanged(const QColor& _color);

public slots:
    void OnSliderChanged();
    void OnSliderReleased();
    void OnColorTextChanged();
};

#endif // QCOLOREDITOR_H
