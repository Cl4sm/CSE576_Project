int
httpFindHeader(AtomPtr header, const char *headers, int hlen,
               int *value_begin_return, int *value_end_return)
{
    int len = header->length;
    int i = 0;

    while(i + len + 1 < hlen) {
        if(headers[i + len] == ':' &&
           lwrcmp(headers + i, header->string, len) == 0) {
            int j = i + len + 1, k;
            while(j < hlen && headers[j] == ' ')
                j++;
            k = j;
            while(k < hlen && headers[k] != '\n' && headers[k] != '\r')
                k++;
            *value_begin_return = j;
            *value_end_return = k;
            return 1;
        } else {
            while(i < hlen && headers[i] != '\n' && headers[i] != '\r')
                i++;
            i++;
            if(i < hlen && headers[i] == '\n')
                i++;
        }
    }
    return 0;
}