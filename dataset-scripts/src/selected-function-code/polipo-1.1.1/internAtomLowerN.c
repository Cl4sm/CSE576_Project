AtomPtr
internAtomLowerN(const char *string, int n)
{
    char *s;
    char buf[100];
    AtomPtr atom;

    if(n < 0 || n >= 50000)
        return NULL;

    if(n < 100) {
        s = buf;
    } else {
        s = malloc(n);
        if(s == NULL)
            return NULL;
    }

    lwrcpy(s, string, n);
    atom = internAtomN(s, n);
    if(s != buf) free(s);
    return atom;
}