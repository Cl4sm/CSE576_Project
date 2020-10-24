int
parse_number (const char **const strptr, unsigned int *const numptr)
{
    if (!isdigit ((unsigned char)**strptr) && **strptr != '.')
        return -1;

    *numptr = 0;

    while (isdigit ((unsigned char)**strptr))
    {
        *numptr = *numptr * 10 + (*(*strptr)++ - '0');
        if (*numptr >= DENOM)
            return -1;
    }

    if (**strptr == '%' || **strptr == '.')
    {
        ++*strptr;
        if (*numptr > 100)
            return -1;

        *numptr *= (DENOM / 100);

        if (*(*strptr - 1) == '.')
        {
            unsigned int value = DENOM / 1000;

            while (isdigit ((unsigned char)**strptr))
            {
                *numptr += value * (*(*strptr)++ - '0');
                value /= 10;
            }

            if (**strptr == '%')
                ++*strptr;
            else
                return -1;

            if (*numptr > DENOM)
                return -1;
        }

        return 0;
    }

    return 1;
}