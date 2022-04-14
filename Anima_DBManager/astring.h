#ifndef ASTRING_H
#define ASTRING_H

#include "attribute.h"

class AString : public Attribute
{
private:
    QString value;

public:
    AString(const AttributeParam* _sharedParam);
    AString(const AttributeParam* _sharedParam, QString _value);

    virtual Type GetType() const override { return Type::String; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASTRING_H
