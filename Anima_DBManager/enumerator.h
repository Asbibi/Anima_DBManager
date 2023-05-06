#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <QObject>
#include <QString>
#include <QColor>
#include <vector>
#include <fstream>

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

    bool GetUseColor() const;
    QString GetColorHex(int _index) const;
    const QColor& GetColor(int _index) const;
    void SetColorToWidget(int _index, QWidget* _widget) const;

    void SetName(const QString& _name);
    void AddValue(const QString& _value);
    void AddValue(const QString& _value, const QColor* _color);
    void RemoveValue(const QString& _value);
    void RemoveValue(int _index);

    void SaveEnum_CSV(std::ofstream& file) const;
};

#endif // ENUMERATOR_H
