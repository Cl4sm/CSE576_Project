static int
createFile(const char *name, int path_start)
{
    int fd;
    char buf[1024];
    int n;
    int rc;

    if(name[path_start] == '/')
        path_start++;

    if(path_start < 2 || name[path_start - 1] != '/' ) {
        do_log(L_ERROR, "Incorrect name %s (%d).\n", name, path_start);
        return -1;
    }

    fd = open(name, O_RDWR | O_CREAT | O_EXCL | O_BINARY,
	      diskCacheFilePermissions);
    if(fd >= 0)
        return fd;
    if(errno != ENOENT) {
        do_log_error(L_ERROR, errno, "Couldn't create disk file %s", name);
        return -1;
    }
    
    n = path_start;
    while(name[n] != '\0' && n < 1024) {
        while(name[n] != '/' && name[n] != '\0' && n < 512)
            n++;
        if(name[n] != '/' || n >= 1024)
            break;
        memcpy(buf, name, n + 1);
        buf[n + 1] = '\0';
        rc = mkdir(buf, diskCacheDirectoryPermissions);
        if(rc < 0 && errno != EEXIST) {
            do_log_error(L_ERROR, errno, "Couldn't create directory %s", buf);
            return -1;
        }
        n++;
    }
    fd = open(name, O_RDWR | O_CREAT | O_EXCL | O_BINARY,
	      diskCacheFilePermissions);
    if(fd < 0) {
        do_log_error(L_ERROR, errno, "Couldn't create file %s", name);
        return -1;
    }

    return fd;
}