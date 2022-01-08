#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <QString>
#include <QColor>
#include <vector>

class Enumerator
{
protected:
    std::vector<QString>    values;
    std::vector<QColor>     colors;

public:
    Enumerator();

    int GetValueNumber() const;
    QString GetValue(int _index) const;

    void AddValue(QString _value);
    void AddValue(QString _value, QColor _color);
};

#endif // ENUMERATOR_H
