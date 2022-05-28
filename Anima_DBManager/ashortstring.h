#ifndef ASHORTSTRING_H
#define ASHORTSTRING_H

#include "attribute.h"

class AShortString : public Attribute
{
private:
    QString value;

public:
    AShortString(const AttributeParam* _sharedParam);
    AShortString(const AttributeParam* _sharedParam, QString _value);

    virtual Type GetType() const override { return Type::ShortString; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASHORTSTRING_H
