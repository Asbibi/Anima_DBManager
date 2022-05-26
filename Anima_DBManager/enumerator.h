#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <QString>
#include <QColor>
#include <vector>

class QWidget;

class Enumerator
{
protected:
    QString                 name;
    std::vector<QString>    values;
    std::vector<QColor>     colors;

public:
    Enumerator(QString _name);
    Enumerator(QString _name, QString* _values, int _count);
    Enumerator(QString _name, QString* _values, QColor* _colors, int _count); // colors and values must have same size
    Enumerator(QString _name, std::vector<QString> _values);
    Enumerator(QString _name, std::vector<QString> _values, std::vector<QColor> _colors);

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
