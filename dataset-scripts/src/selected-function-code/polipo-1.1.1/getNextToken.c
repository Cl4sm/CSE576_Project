static int
getNextToken(const char *restrict buf, int i, int *x_return, int *y_return)
{
    int x, y;
 again:
    while(buf[i] == ' ' || buf[i] == '\t')
        i++;
    if(buf[i] == '(') {
        i++;
        while(buf[i] != ')') {
            if(buf[i] == '\n' || buf[i] == '\r')
                return -1;
            if(buf[i] == '\\' && buf[i + 1] != '\n' && buf[i + 1] != '\r')
                buf += 2;
            else
                buf++;
        }
        goto again;
    }
    if(buf[i] == '\n') {
        if(buf[i + 1] == ' ' || buf[i + 1] == '\t') {
            i += 2;
            goto again;
        } else {
            return -1;
        }
    }
    if(buf[i] == '\r') {
        if(buf[i + 1] == '\n' && (buf[i + 2] == ' ' || buf[i + 2] == '\t')) {
            i += 3;
            goto again;
        } else {
            return -1;
        }
    }
    x = i;
    while(buf[i] > 32 && buf[i] < 127) {
        switch(buf[i]) {
        case '(': case ')': case '<': case '>': case '@':
        case ',': case ';': case ':': case '\\': case '/':
        case '[': case ']': case '?': case '=':
        case '{': case '}': case ' ': case '\t':
            goto out;
        default:
            i++;
        }
    }
 out:
    y = i;

    *x_return = x;
    *y_return = y;

    return y;
}