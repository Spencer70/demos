#include "Faces.h"

int aColors_mask(const aColors *ac)
{
     int mask = 0;
     for (int i = 0; i < ac->count; i++)
        mask |= (1 << ac->colors[i]);
     return mask;
}

bool aColors_Contains(const aColors *set, const aColors *subset, bool matchAll)
{
    int mset = aColors_mask(set);
    int msubset = aColors_mask(subset);
    mset &= msubset;
    if (mset == 0 || matchAll && mset != msubset)
        return false;
    return true;
}

// Equal arrays until count
bool aColors_Equals(const aColors *ac1, const aColors *ac2)
{
    if (ac1->count != ac2->count)
        return false;
    for (int i = 0; i < ac1->count; i++)
        if (ac1->colors[i] != ac2->colors[i])
            return false;
    return true;
}

// Unordered but same colors in both arrays
bool aColors_EqualSet(const aColors *ac1, const aColors *ac2)
{
    if (ac1->count != ac2->count)
        return false;
    return (aColors_mask(ac1) == aColors_mask(ac2));
}

int aColors_Index(const aColors *ac, Colors c)
{
    for (int i = 0; i < ac->count; i++)
        if (ac->colors[i] == c)
            return i;
    return -1;
}

