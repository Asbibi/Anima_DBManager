#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

class Language : public QObject
{
    Q_OBJECT
protected:
    QString name;
    QString abbrev;

public:
    Language(const QString& _abbrev, const QString& _name);
    Language(const Language& _other);
    void operator=(const Language& _other);

    const QString& GetName() const;
    const QString& GetAbbrev() const;
};

#endif // LANGUAGE_H
