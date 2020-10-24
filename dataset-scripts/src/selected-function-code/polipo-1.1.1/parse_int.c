static int
parse_int(const char *buf, int i, int len, int *val_return)
{
    int val, d;

    if(i >= len)
        return -1;

    val = d2i(buf[i]);
    if(val < 0)
        return -1;
    else
        i++;

    while(i < len) {
        d = d2i(buf[i]);
        if(d < 0)
            break;
        val = val * 10 + d;
        i++;
    }
    *val_return = val;
    return i;
}