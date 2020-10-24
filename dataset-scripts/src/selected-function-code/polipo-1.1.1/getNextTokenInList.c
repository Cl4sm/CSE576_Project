static int
getNextTokenInList(const char *restrict buf, int i, 
                   int *x_return, int *y_return,
                   int *z_return, int *t_return,
                   int *end_return)
{
    int j, x, y, z = -1, t = -1, end;
    j = getNextToken(buf, i, &x, &y);
    if(j < 0)
        return -1;
    while(buf[j] == ' ' || buf[j] == '\t')
        j++;

    if(buf[j] == '=') {
        j++;
        while(buf[j] == ' ' || buf[j] == '\t')
            j++;
        z = j;
        while(buf[j] != ',' && buf[j] != '\n' && buf[j] != '\r')
            j++;
    }

    if(buf[j] == '\n' || buf[j] == '\r') {
        if(buf[j] == '\r') {
            if(buf[j + 1] != '\n')
                return -1;
            j += 2;
        } else
            j++;
        end = 1;
        if(buf[j] == ' ' || buf[j] == '\t') {
            while(buf[j] == ' ' || buf[j] == '\t')
                j++;
            end = 0;
        }
    } else if(buf[j] == ',') {
        j++;
        while(buf[j] == ' ' || buf[j] == '\t')
            j++;
        end = 0;
    } else {
        return -1;
    }

    *x_return = x;
    *y_return = y;
    if(z_return)
        *z_return = z;
    if(t_return)
        *t_return = t;
    *end_return = end;
    return j;
}