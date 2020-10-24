int
htmlString(char *buf, int n, int len, char *s, int slen)
{
    int i = 0;
    while(i < slen && n + 5 < len) {
        switch(s[i]) {
        case '&':
            buf[n++] = '&'; buf[n++] = 'a'; buf[n++] = 'm'; buf[n++] = 'p';
            buf[n++] = ';';
            break;
        case '<':
            buf[n++] = '&'; buf[n++] = 'l'; buf[n++] = 't'; buf[n++] = ';';
            break;
        case '>':
            buf[n++] = '&'; buf[n++] = 'g'; buf[n++] = 't'; buf[n++] = ';';
            break;
        case '"':
            buf[n++] = '&'; buf[n++] = 'q'; buf[n++] = 'u'; buf[n++] = 'o';
            buf[n++] = 't'; buf[n++] = ';';
            break;
        case '\0':
            break;
        default:
            buf[n++] = s[i];
        }
        i++;
    }
    return n;
}