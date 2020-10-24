int
findEndOfHeaders(const char *restrict buf, int from, int to, int *body_return) 
{
    int i = from;
    int eol = 0;
    while(i < to) {
        if(buf[i] == '\n') {
            if(eol) {
                *body_return = i + 1;
                return eol;
            }
            eol = i;
            i++;
        } else if(buf[i] == '\r') {
            if(i < to - 1 && buf[i + 1] == '\n') {
                if(eol) {
                    *body_return = eol;
                    return i + 2;
                }
                eol = i;
                i += 2;
            } else {
                eol = 0;
                i++;
            }
        } else {
            eol = 0;
            i++;
        }
    }
    return -1;
}