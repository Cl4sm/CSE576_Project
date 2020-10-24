static int
parseAtom(char *buf, int offset, AtomPtr *value_return, int insensitive)
{
    int y0, i, j, k;
    AtomPtr atom;
    int escape = 0;
    char *s;

    i = offset;
    if(buf[i] == '\"') {
        i++;
        y0 = i;
        while(buf[i] != '\"' && buf[i] != '\n' && buf[i] != '\0') {
            if(buf[i] == '\\' && buf[i + 1] != '\0') {
                escape = 1;
                i += 2;
            } else
                i++;
        }
        if(buf[i] != '\"')
            return -1;
        j = i + 1;
    } else {
        y0 = i;
        while(letter(buf[i]) || digit(buf[i]) || 
              buf[i] == '_' || buf[i] == '-' || buf[i] == '~' ||
              buf[i] == '.' || buf[i] == ':' || buf[i] == '/')
            i++;
        j = i;
    }

    if(escape) {
        s = malloc(i - y0);
        if(s == NULL) return -1;
        k = 0;
        j = y0;
        while(j < i) {
            if(buf[j] == '\\' && j <= i - 2) {
                s[k++] = buf[j + 1];
                j += 2;
            } else
                s[k++] = buf[j++];
        }
        if(insensitive)
            atom = internAtomLowerN(s, k);
        else
            atom = internAtomN(s, k);
        free(s);
        j++;
    } else {
        if(insensitive)
            atom = internAtomLowerN(buf + y0, i - y0);
        else
            atom = internAtomN(buf + y0, i - y0);
    }
    *value_return = atom;
    return j;
}