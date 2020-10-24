AtomPtr
internAtomF(const char *format, ...)
{
    char *s;
    char buf[150];
    int n;
    va_list args;
    AtomPtr atom = NULL;

    va_start(args, format);
    n = vsnprintf(buf, 150, format, args);
    va_end(args);
    if(n >= 0 && n < 150) {
        atom = internAtomN(buf, n);
    } else {
        va_start(args, format);
        s = vsprintf_a(format, args);
        va_end(args);
        if(s != NULL) {
            atom = internAtom(s);
            free(s);
        }
    }
    return atom;
}