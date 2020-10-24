static AtomPtr
internAtomErrorV(int e, const char *f, va_list args)
{
    
    char *es = pstrerror(e);
    AtomPtr atom;
    char *s1, *s2;
    int n, rc;
    va_list args_copy;

    if(f) {
        va_copy(args_copy, args);
        s1 = vsprintf_a(f, args_copy);
        va_end(args_copy);
        if(s1 == NULL)
            return NULL;
        n = strlen(s1);
    } else {
        s1 = NULL;
        n = 0;
    }

    s2 = malloc(n + 70);
    if(s2 == NULL) {
        free(s1);
        return NULL;
    }
    if(s1) {
        strcpy(s2, s1);
        free(s1);
    }

    rc = snprintf(s2 + n, 69, f ? ": %s" : "%s", es);
    if(rc < 0 || rc >= 69) {
        free(s2);
        return NULL;
    }

    atom = internAtomN(s2, n + rc);
    free(s2);
    return atom;
}