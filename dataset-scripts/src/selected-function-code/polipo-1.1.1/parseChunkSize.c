int
parseChunkSize(const char *restrict buf, int i, int end,
               int *chunk_size_return)
{
    int v, d;
    v = h2i(buf[i]);
    if(v < 0)
        return -1;

    i++;

    while(i < end) {
        d = h2i(buf[i]);
        if(d < 0)
            break;
        v = v * 16 + d;
        i++;
    }

    while(i < end) {
        if(buf[i] == ' ' || buf[i] == '\t')
            i++;
        else
            break;
    }

    if(i >= end - 1)
        return 0;

    if(buf[i] != '\r' || buf[i + 1] != '\n')
        return -1;

    i += 2;

    if(v == 0) {
        if(i >= end - 1)
            return 0;
        if(buf[i] != '\r') {
            do_log(L_ERROR, "Trailers present!\n");
            return -1;
        }
        i++;
        if(buf[i] != '\n')
            return -1;
        i++;
    }

    *chunk_size_return = v;
    return i;
}