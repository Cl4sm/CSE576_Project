int
checkVia(AtomPtr name, AtomPtr via)
{
    int i;
    char *v;
    if(via == NULL || via->length == 0)
        return 1;

    v = via->string;

    i = 0;
    while(i < via->length) {
        while(v[i] == ' ' || v[i] == '\t' || v[i] == ',' ||
              v[i] == '\r' || v[i] == '\n' ||
              digit(v[i]) || v[i] == '.')
            i++;
        if(i + name->length > via->length)
            break;
        if(memcmp(v + i, name->string, name->length) == 0) {
            char c = v[i + name->length];
            if(c == '\0' || c == ' ' || c == '\t' || c == ',' ||
               c == '\r' || c == '\n')
                return 0;
        }
        i++;
        while(letter(v[i]) || digit(v[i]) || v[i] == '.')
            i++;
    }
    return 1;
}