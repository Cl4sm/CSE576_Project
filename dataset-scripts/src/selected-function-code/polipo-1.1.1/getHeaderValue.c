static int
getHeaderValue(const char *restrict buf, int start, 
               int *value_start_return, int *value_end_return)
{
    int i, j, k;

    while(buf[start] == ' ' || buf[start] == '\t')
        start++;
    i = start;
 again:
    j = skipToEol(buf, i, &k);
    if(j < 0)
        return -1;
    if(buf[j] == ' ' || buf[j] == '\t') {
        i = j + 1;
        goto again;
    }
    *value_start_return = start;
    *value_end_return = k;
    return j;
}