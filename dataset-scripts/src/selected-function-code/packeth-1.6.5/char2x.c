signed int char2x(char *p) 
{
    unsigned char x=0;

    if ( (*p >= '0') && (*p <= '9')) {
        x = ((*p) - 48) * 16;
    }
    else if ((*p >= 'A') && (*p <= 'F')) {
        x = ((*p) - 55) * 16;
    }
    else if ((*p >= 'a') && (*p <= 'f')) {
        x = ((*p) - 87) * 16;
    }
    else {
        return -1;
    }
    p++;
    if ( (*p >= '0') && (*p <= '9')) {
        x = x + ((*p) - 48);
    }
    else if ((*p >= 'A') && (*p <= 'F')) {
        x = x + ((*p) - 55);
    }
    else if ((*p >= 'a') && (*p <= 'f')) {
        x = x + ((*p) - 87);
    }
    else {
        return -1;
    }
    return (int)x;
}