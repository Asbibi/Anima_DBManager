#ifndef ABOOL_H
#define ABOOL_H

#include "attribute.h"

class ABool : public Attribute
{
private:
    bool value;

public:
    ABool();
    ABool(bool _value);

    virtual Type GetType() const override { return Type::Bool; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;


    bool GetValue() const {return value;};
    void SetValue(bool _value) {value = _value;};
};

#endif // ABOOL_H