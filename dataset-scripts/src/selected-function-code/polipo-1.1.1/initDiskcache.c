void
initDiskcache()
{
    int rc;

    diskCacheRoot = expandTilde(maybeAddSlash(diskCacheRoot));
    rc = checkRoot(diskCacheRoot);
    if(rc <= 0) {
        switch(rc) {
        case 0: break;
        case -1: do_log_error(L_WARN, errno, "Disabling disk cache"); break;
        case -2: 
            do_log(L_WARN, "Disabling disk cache: path %s is not absolute.\n",
                   diskCacheRoot->string); 
            break;
        default: abort();
        }
        releaseAtom(diskCacheRoot);
        diskCacheRoot = NULL;
    }

    localDocumentRoot = expandTilde(maybeAddSlash(localDocumentRoot));
    rc = checkRoot(localDocumentRoot);
    if(rc <= 0) {
        switch(rc) {
        case 0: break;
        case -1: do_log_error(L_WARN, errno, "Disabling local tree"); break;
        case -2: 
            do_log(L_WARN, "Disabling local tree: path is not absolute.\n"); 
            break;
        default: abort();
        }
        releaseAtom(localDocumentRoot);
        localDocumentRoot = NULL;
    }
}