int
httpParseServerFirstLine(const char *restrict buf, 
                         int *status_return,
                         int *version_return,
                         AtomPtr *message_return)
{
    int i = 0;
    int x, y, eol;
    int status;
    int version = HTTP_UNKNOWN;
    
    i = getNextWord(buf, 0, &x, &y);
    if(i < 0)
        return -1;
    if(y == x + 8 && memcmp(buf + x, "HTTP/1.0", 8) == 0)
        version = HTTP_10;
    else if(y >= x + 8 && memcmp(buf + x, "HTTP/1.", 7) == 0)
        version = HTTP_11;
    else
        version = HTTP_UNKNOWN;

    i = getNextWord(buf, y + 1, &x, &y);
    if(i < 0) return -1;
    if(y == x + 3)
        status = atol(buf + x);
    else return -1;

    i = skipToEol(buf, y, &eol);
    if(i < 0) return -1;
        
    *status_return = status;
    *version_return = version;
    if(message_return) {
        /* Netscape enterprise bug */
        if(eol > y)
            *message_return = internAtomN(buf + y + 1, eol - y - 1);
        else
            *message_return = internAtom("No message");
    }
    return i;
}