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

    virtual Attribute* CreateDuplica()              const = 0;
    virtual QString GetDisplayedText()              const = 0;
    virtual void WriteValue_CSV(std::ofstream& file) const = 0;
};

#endif // ATTRIBUTE_H
