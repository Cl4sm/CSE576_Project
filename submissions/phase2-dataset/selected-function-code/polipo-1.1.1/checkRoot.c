static int
checkRoot(AtomPtr root)
{
    struct stat ss;
    int rc;

    if(!root || root->length == 0)
        return 0;

#ifdef WIN32  /* Require "x:/" or "x:\\" */
    rc = isalpha(root->string[0]) && (root->string[1] == ':') &&
         ((root->string[2] == '/') || (root->string[2] == '\\'));
    if(!rc) {
        return -2;
    }
#else
    if(root->string[0] != '/') {
        return -2;
    }
#endif

    rc = stat(root->string, &ss);
    if(rc < 0)
        return -1;
    else if(!S_ISDIR(ss.st_mode)) {
        errno = ENOTDIR;
        return -1;
    }
    return 1;
}