static int
parseHeaderLine(const char *restrict buf, int start,
                int *name_start_return, int *name_end_return,
                int *value_start_return, int *value_end_return)
{
    int i;
    int name_start, name_end, value_start, value_end;

    if(buf[start] == '\n') {
        *name_start_return = -1;
        return start + 1;
    }
    if(buf[start] == '\r' && buf[start + 1] == '\n') {
        *name_start_return = -1;
        return start + 2;
    }

    i = getNextToken(buf, start, &name_start, &name_end);
    if(i < 0 || buf[i] != ':')
        goto syntax;
    i++;
    while(buf[i] == ' ' || buf[i] == '\t')
        i++;

    i = getHeaderValue(buf, i, &value_start, &value_end);
    if(i < 0)
        goto syntax;

    *name_start_return = name_start;
    *name_end_return = name_end;
    *value_start_return = value_start;
    *value_end_return = value_end;
    return i;

 syntax:
    i = start;
    while(1) {
        if(buf[i] == '\n') {
            i++;
            break;
        }
        if(buf[i] == '\r' && buf[i + 1] == '\n') {
            i += 2;
            break;
        }
        i++;
    }
    *name_start_return = -2;
    return i;
}