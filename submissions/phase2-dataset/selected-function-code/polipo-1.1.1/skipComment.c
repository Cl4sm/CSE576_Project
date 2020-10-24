static int
skipComment(const char *restrict buf, int i)
{
    assert(buf[i] == '(');

    i++;
    while(1) {
        if(buf[i] == '\\' && buf[i + 1] == ')') i+=2;
        else if(buf[i] == ')') return i + 1;
        else if(buf[i] == '\n') {
            if(buf[i + 1] == ' ' || buf[i + 1] == '\t')
                i += 2;
            else
                return -1;
        } else if(buf[i] == '\r') {
            if(buf[i + 1] != '\n') return -1;
            if(buf[i + 2] == ' ' || buf[i + 2] == '\t')
                i += 3;
            else
                return -1;
        } else {
            i++;
        }
    }
    return i;
}