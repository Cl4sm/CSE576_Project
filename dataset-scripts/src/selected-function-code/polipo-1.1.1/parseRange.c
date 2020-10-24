static int
parseRange(const char *restrict buf, int i, 
           int *from_return, int *to_return)
{
    int j;
    int from, to;

    i = skipWhitespace(buf, i);
    if(i < 0)
        return -1;
    if(!token_compare(buf, i, i + 6, "bytes="))
        return -1;
    i += 6;
    i = skipWhitespace(buf, i);
    if(buf[i] == '-') {
        from = 0;
    } else {
        i = parseInt(buf, i, &from);
        if(i < 0) return -1;
    }
    if(buf[i] != '-')
        return -1;
    i++;
    j = parseInt(buf, i, &to);
    if(j < 0) 
        to = -1;
    else {
        to = to + 1;
        i = j;
    }
    j = skipEol(buf, i);
    if(j < 0) return -1;
    *from_return = from;
    *to_return = to;
    return i;
}