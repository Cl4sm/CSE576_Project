static int
parseContentRange(const char *restrict buf, int i, 
                  int *from_return, int *to_return, int *full_len_return)
{
    int j;
    int from, to, full_len;

    i = skipWhitespace(buf, i);
    if(i < 0) return -1;
    if(!token_compare(buf, i, i + 5, "bytes")) {
        do_log(L_WARN, "Incorrect Content-Range header -- chugging along.\n");
    } else {
        i += 5;
    }
    i = skipWhitespace(buf, i);
    if(buf[i] == '*') {
        from = 0;
        to = -1;
        i++;
    } else {
        i = parseInt(buf, i, &from);
        if(i < 0) return -1;
        if(buf[i] != '-') return -1;
        i++;
        i = parseInt(buf, i, &to);
        if(i < 0) return -1;
        to = to + 1;
    }
    if(buf[i] != '/')
        return -1;
    i++;
    if(buf[i] == '*')
        full_len = -1;
    else {
        i = parseInt(buf, i, &full_len);
        if(i < 0) return -1;
    }
    j = skipEol(buf, i);
    if(j < 0)
        return -1;

    *from_return = from;
    *to_return = to;
    *full_len_return = full_len;
    return i;
}