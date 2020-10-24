static int
getNextETag(const char * restrict buf, int i, 
            int *x_return, int *y_return, int *weak_return)
{
    int weak = 0;
    int x, y;
    while(buf[i] == ' ' || buf[i] == '\t')
        i++;
    if(buf[i] == 'W' && buf[i + 1] == '/') {
        weak = 1;
        i += 2;
    }
    if(buf[i] == '"')
        i++;
    else
        return -1;

    x = i;
    while(buf[i] != '"') {
        if(buf[i] == '\r' || buf[i] == '\n')
            return -1;
        i++;
    }
    y = i;
    i++;

    *x_return = x;
    *y_return = y;
    *weak_return = weak;
    return i;
}