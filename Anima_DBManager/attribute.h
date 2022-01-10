#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QWidget>
#include <fstream>
#include "attributeparam.h"

class Attribute
{
protected:
    const AttributeParam* sharedParam;

public:
    Attribute(const AttributeParam* _sharedParam);
    virtual ~Attribute() {};

    virtual Attribute* CreateDuplica()                      const = 0;
    virtual QString GetDisplayedText(bool complete = false) const = 0;
    virtual void WriteValue_CSV(std::ofstream& file)        const = 0;
    virtual void SetValueFromText(const QString& text)            = 0;
};

#endif // ATTRIBUTE_H
