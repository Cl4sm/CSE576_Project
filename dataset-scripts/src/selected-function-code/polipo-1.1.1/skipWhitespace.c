static int
skipWhitespace(const char *restrict buf, int i)
{
    while(1) {
        if(buf[i] == ' ' || buf[i] == '\t')
            i++;
        else  if(buf[i] == '(') {
            i = skipComment(buf, i);
            if(i < 0) return -1;
        } else if(buf[i] == '\n') {
            if(buf[i + 1] == ' ' || buf[i + 1] == '\t')
                i += 2;
            else
                return i;
        } else if(buf[i] == '\r' && buf[i + 1] == '\n') {
            if(buf[i + 2] == ' ' || buf[i + 2] == '\t')
                i += 3;
            else
                return i;
        } else
            return i;
    }
}