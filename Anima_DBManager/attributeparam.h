#ifndef ATTRIBUTEPARAM_H
#define ATTRIBUTEPARAM_H


struct AttributeParam
{
    bool ignoreMin = true;
    bool ignoreMax = true;
    int min_i = 0;
    int max_i = 0;      // Max are inclusive for int and float, exclusive for string char count and arrays in general
    float min_f = 0;
    float max_f = 0;    // Max are inclusive for int and float
};

#endif // ATTRIBUTEPARAM_H
