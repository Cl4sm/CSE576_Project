static int list_urls(char *path, apr_pool_t *pool, apr_off_t round)
{
    apr_dir_t *dir;
    apr_finfo_t info;
    apr_size_t len;
    apr_pool_t *p;
    apr_file_t *fd;
    const char *ext, *nextpath;
    char *url;
    apr_uint32_t format;
    disk_cache_info_t disk_info;

    apr_pool_create(&p, pool);

    if (apr_dir_open(&dir, path, p) != APR_SUCCESS) {
        return 1;
    }

    while (apr_dir_read(&info, APR_FINFO_TYPE, dir) == APR_SUCCESS && !interrupted) {

        if (info.filetype == APR_DIR) {
            if (!strcmp(info.name, ".") || !strcmp(info.name, "..")) {
                continue;
            }

            if (list_urls(apr_pstrcat(p, path, "/", info.name, NULL), pool, round)) {
                return 1;
            }
        }

        else if (info.filetype == APR_REG) {

            ext = strchr(info.name, '.');

            if (ext && !strcasecmp(ext, CACHE_HEADER_SUFFIX)) {

                nextpath = apr_pstrcat(p, path, "/", info.name, NULL);

                if (apr_file_open(&fd, nextpath, APR_FOPEN_READ
                        | APR_FOPEN_BINARY, APR_OS_DEFAULT, p) == APR_SUCCESS) {
                    len = sizeof(format);
                    if (apr_file_read_full(fd, &format, len, &len)
                            == APR_SUCCESS) {
                        if (format == DISK_FORMAT_VERSION) {
                            apr_off_t offset = 0;

                            apr_file_seek(fd, APR_SET, &offset);

                            len = sizeof(disk_cache_info_t);

                            if (apr_file_read_full(fd, &disk_info, len, &len)
                                    == APR_SUCCESS) {
                                len = disk_info.name_len;
                                url = apr_palloc(p, len + 1);
                                url[len] = 0;

                                if (apr_file_read_full(fd, url, len, &len)
                                        == APR_SUCCESS) {

                                    if (listextended) {
                                        apr_finfo_t hinfo, dinfo;

                                        /* stat the header file */
                                        if (APR_SUCCESS != apr_file_info_get(
                                                &hinfo, APR_FINFO_SIZE, fd)) {
                                            /* ignore the file */
                                        }
                                        else if (disk_info.has_body && APR_SUCCESS
                                                != apr_stat(
                                                        &dinfo,
                                                        apr_pstrcat(
                                                                p,
                                                                path,
                                                                "/",
                                                                apr_pstrndup(
                                                                        p,
                                                                        info.name,
                                                                        ext
                                                                                - info.name),
                                                                CACHE_DATA_SUFFIX,
                                                                NULL),
                                                        APR_FINFO_SIZE
                                                                | APR_FINFO_IDENT,
                                                        p)) {
                                            /* ignore the file */
                                        }
                                        else if (disk_info.has_body && (dinfo.device
                                                != disk_info.device
                                                || dinfo.inode
                                                        != disk_info.inode)) {
                                            /* ignore the file */
                                        }
                                        else {

                                            apr_file_printf(
                                                    outfile,
                                                    "%s %" APR_SIZE_T_FMT
                                                    " %" APR_SIZE_T_FMT
                                                    " %d %" APR_SIZE_T_FMT
                                                    " %" APR_TIME_T_FMT
                                                    " %" APR_TIME_T_FMT
                                                    " %" APR_TIME_T_FMT
                                                    " %" APR_TIME_T_FMT
                                                    " %d %d\n",
                                                    url,
                                                    round_up((apr_size_t)hinfo.size, round),
                                                    round_up(
                                                            disk_info.has_body ? (apr_size_t)dinfo.size
                                                                    : 0, round),
                                                    disk_info.status,
                                                    disk_info.entity_version,
                                                    disk_info.date,
                                                    disk_info.expire,
                                                    disk_info.request_time,
                                                    disk_info.response_time,
                                                    disk_info.has_body,
                                                    disk_info.header_only);
                                        }
                                    }
                                    else {
                                        apr_finfo_t dinfo;

                                        /* stat the data file */
                                        if (disk_info.has_body && APR_SUCCESS
                                                != apr_stat(
                                                        &dinfo,
                                                        apr_pstrcat(
                                                                p,
                                                                path,
                                                                "/",
                                                                apr_pstrndup(
                                                                        p,
                                                                        info.name,
                                                                        ext
                                                                                - info.name),
                                                                CACHE_DATA_SUFFIX,
                                                                NULL),
                                                        APR_FINFO_SIZE
                                                                | APR_FINFO_IDENT,
                                                        p)) {
                                            /* ignore the file */
                                        }
                                        else if (disk_info.has_body && (dinfo.device
                                                != disk_info.device
                                                || dinfo.inode
                                                        != disk_info.inode)) {
                                            /* ignore the file */
                                        }
                                        else {
                                            apr_file_printf(outfile, "%s\n",
                                                    url);
                                        }
                                    }
                                }

                                break;
                            }
                        }
                    }
                    apr_file_close(fd);

                }
            }
        }

    }

    apr_dir_close(dir);

    if (interrupted) {
        return 1;
    }

    apr_pool_destroy(p);

    if (benice) {
        apr_sleep(NICE_DELAY);
    }

    if (interrupted) {
        return 1;
    }

    return 0;
}