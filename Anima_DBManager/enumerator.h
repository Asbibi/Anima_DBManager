#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <QObject>
#include <QString>
#include <QColor>
#include <vector>

class QWidget;

class Enumerator : public QObject
{
protected:
    QString                 name;
    std::vector<QString>    values;
    std::vector<QColor>     colors;

public:
    Enumerator(const QString& _name);
    Enumerator(const QString& _name, const QString* _values, const int _count);
    Enumerator(const QString& _name, const QString* _values, const QColor* _colors, const int _count); // colors and values must have same size
    Enumerator(const QString& _name, const std::vector<QString>& _values);
    Enumerator(const QString& _name, const std::vector<QString>& _values, const std::vector<QColor>& _colors);
    Enumerator(const Enumerator& _another);

    void operator=(const Enumerator& _another);

    QString GetName() const;
    int GetValueCount() const;
    QString GetValue(int _index) const;
    int GetValueIndex(const QString& _value) const;
    void SetColorToWidget(int _index, QWidget* _widget) const;

    void AddValue(const QString& _value);
    void AddValue(const QString& _value, const QColor* _color);
    void RemoveValue(const QString& _value);
    void RemoveValue(int _index);
};

#endif // ENUMERATOR_H
