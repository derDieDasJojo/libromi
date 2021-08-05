
#ifndef _AZHOO_I_FILTER_H
#define _AZHOO_I_FILTER_H

#include <stdint.h>

class IFilter
{
public:

        virtual ~IFilter() = default;
        virtual int16_t compute(int16_t x) = 0;
};

#endif // _AZHOO_I_FILTER_H
