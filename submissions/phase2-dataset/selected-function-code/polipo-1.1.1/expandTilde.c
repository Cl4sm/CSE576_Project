AtomPtr
expandTilde(AtomPtr filename)
{
    char *buf;
    char *home;
    int len;
    AtomPtr ret;

    if(filename == NULL || filename->length < 1 ||
       filename->string[0] != '~' || filename->string[1] != '/')
        return filename;
    
    home = getenv("HOME");
    if(home == NULL) {
        return NULL;
    }
    len = strlen(home);
    buf = malloc(len + 1 + 1 + filename->length - 2);
    if(buf == NULL) {
        do_log(L_ERROR, "Could not allocate buffer.\n");
        return NULL;
    }

    memcpy(buf, home, len);
    if(buf[len - 1] != '/')
        buf[len++] = '/';
    memcpy(buf + len, filename->string + 2, filename->length - 2);
    len += filename->length - 2;
    ret = internAtomN(buf, len);
    free(buf);
    if(ret != NULL)
        releaseAtom(filename);
    return ret;
}