int
httpParseClientFirstLine(const char *restrict buf, int offset,
                         int *method_return,
                         AtomPtr *url_return,
                         int *version_return)
{
    int i = 0;
    int x, y;
    int method;
    AtomPtr url;
    int version = HTTP_UNKNOWN;
    int eol;

    i = offset;
    i = getNextWord(buf, i, &x, &y);
    if(i < 0) return -1;
    if(y == x + 3 && memcmp(buf + x, "GET", 3) == 0)
        method = METHOD_GET;
    else if(y == x + 4 && memcmp(buf + x, "HEAD", 4) == 0)
        method = METHOD_HEAD;
    else if(y == x + 4 && memcmp(buf + x, "POST", 4) == 0)
        method = METHOD_POST;
    else if(y == x + 3 && memcmp(buf + x, "PUT", 3) == 0)
        method = METHOD_PUT;
    else if(y == x + 7 && memcmp(buf + x, "CONNECT", 7) == 0)
        method = METHOD_CONNECT;
    else
        method = METHOD_UNKNOWN;

    i = getNextWord(buf, y + 1, &x, &y);
    if(i < 0) return -1;

    url = internAtomN(buf + x, y - x);

    i = getNextWord(buf, y + 1, &x, &y);
    if(i < 0) {
        releaseAtom(url);
        return -1;
    }

    if(y == x + 8) {
        if(memcmp(buf + x, "HTTP/1.", 7) != 0)
            version = HTTP_UNKNOWN;
        else if(buf[x + 7] == '0')
            version = HTTP_10;
        else if(buf[x + 7] >= '1' && buf[x + 7] <= '9')
            version = HTTP_11;
        else
            version = HTTP_UNKNOWN;
    }

    eol = skipEol(buf, y);
    if(eol < 0) return -1;
        
    *method_return = method;
    if(url_return)
        *url_return = url;
    else
        releaseAtom(url);
    *version_return = version;
    return eol;
}