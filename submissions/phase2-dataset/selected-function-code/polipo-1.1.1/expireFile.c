static long int
expireFile(char *filename, struct stat *sb,
           int *considered, int *unlinked, int *truncated)
{
    DiskObjectPtr dobject = NULL;
    time_t t;
    int fd, rc;
    long int ret = sb->st_size;

    if(!preciseExpiry) {
        t = sb->st_mtime;
        if(t > current_time.tv_sec + 1) {
            do_log(L_WARN, "File %s has access time in the future.\n",
                   filename);
            t = current_time.tv_sec;
        }
        
        if(t > current_time.tv_sec - diskCacheUnlinkTime &&
           (sb->st_size < diskCacheTruncateSize ||
            t > current_time.tv_sec - diskCacheTruncateTime))
            return ret;
    }
    
    (*considered)++;

    dobject = readDiskObject(filename, sb);
    if(!dobject) {
        do_log(L_ERROR, "Incorrect disk entry %s -- removing.\n",
               scrub(filename));
        rc = unlink(filename);
        if(rc < 0) {
            do_log_error(L_ERROR, errno,
                         "Couldn't unlink %s", scrub(filename));
            return ret;
        } else {
            (*unlinked)++;
            return 0;
        }
    }
    
    t = dobject->access;
    if(t < 0) t = dobject->age;
    if(t < 0) t = dobject->date;
    
    if(t > current_time.tv_sec)
        do_log(L_WARN, 
               "Disk entry %s (%s) has access time in the future.\n",
               scrub(dobject->location), scrub(dobject->filename));
    
    if(t < current_time.tv_sec - diskCacheUnlinkTime) {
        rc = unlink(dobject->filename);
        if(rc < 0) {
            do_log_error(L_ERROR, errno, "Couldn't unlink %s",
                         scrub(filename));
        } else {
            (*unlinked)++;
            ret = 0;
        }
    } else if(dobject->size > 
              diskCacheTruncateSize + 4 * dobject->body_offset && 
              t < current_time.tv_sec - diskCacheTruncateTime) {
        /* We need to copy rather than simply truncate in place: the
           latter would confuse a running polipo. */
        fd = open(dobject->filename, O_RDONLY | O_BINARY, 0);
        rc = unlink(dobject->filename);
        if(rc < 0) {
            do_log_error(L_ERROR, errno, "Couldn't unlink %s",
                         scrub(filename));
            close(fd);
            fd = -1;
        } else {
            (*unlinked)++;
            copyFile(fd, dobject->filename,
                     dobject->body_offset + diskCacheTruncateSize);
            close(fd);
            (*unlinked)--;
            (*truncated)++;
            ret = sb->st_size - dobject->body_offset + diskCacheTruncateSize;
        }
    }
    free(dobject->location);
    free(dobject->filename);
    free(dobject);
    return ret;
}