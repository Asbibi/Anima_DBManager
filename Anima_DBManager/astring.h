#ifndef ASTRING_H
#define ASTRING_H

#include "attribute.h"

class AString : public Attribute
{
public:
    QString value;

    AString(const AttributeParam* _sharedParam);
    AString(const AttributeParam* _sharedParam, QString _value);

    virtual Attribute* CreateDuplica()              const override;
    virtual QString GetDisplayedText()              const override;
    virtual void WriteValue_CSV(std::ofstream& file) const override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASTRING_H
