void
indexDiskObjects(FILE *out, const char *root, int recursive)
{
    int n, i, isdir;
    DIR *dir;
    struct dirent *dirent;
    char buf[1024];
    char *fts_argv[2];
    FTS *fts;
    FTSENT *fe;
    DiskObjectPtr dobjects = NULL;
    char *of = root[0] == '\0' ? "" : " of ";

    fprintf(out, "<!DOCTYPE HTML PUBLIC "
            "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
            "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
            "<html><head>\n"
            "<title>%s%s%s</title>\n"
            "</head><body>\n"
            "<h1>%s%s%s</h1>\n",
            recursive ? "Recursive index" : "Index", of, root,
            recursive ? "Recursive index" : "Index", of, root);

    if(diskCacheRoot == NULL || diskCacheRoot->length <= 0) {
        fprintf(out, "<p>No <tt>diskCacheRoot</tt>.</p>\n");
        goto trailer;
    }

    if(diskCacheRoot->length >= 1024) {
        fprintf(out,
                "<p>The value of <tt>diskCacheRoot</tt> is "
                "too long (%d).</p>\n",
                diskCacheRoot->length);
        goto trailer;
    }

    if(strlen(root) < 8) {
        memcpy(buf, diskCacheRoot->string, diskCacheRoot->length);
        buf[diskCacheRoot->length] = '\0';
        n = diskCacheRoot->length;
    } else {
        n = urlDirname(buf, 1024, root, strlen(root));
    }
    if(n > 0) {
        if(recursive) {
            dir = NULL;
            fts_argv[0] = buf;
            fts_argv[1] = NULL;
            fts = fts_open(fts_argv, FTS_LOGICAL, NULL);
            if(fts) {
                while(1) {
                    fe = fts_read(fts);
                    if(!fe) break;
                    if(fe->fts_info != FTS_DP)
                        dobjects =
                            processObject(dobjects,
                                          fe->fts_path,
                                          fe->fts_info == FTS_NS ||
                                          fe->fts_info == FTS_NSOK ?
                                          fe->fts_statp : NULL);
                }
                fts_close(fts);
            }
        } else {
            dir = opendir(buf);
            if(dir) {
                while(1) {
                    dirent = readdir(dir);
                    if(!dirent) break;
                    if(n + strlen(dirent->d_name) < 1024) {
                        strcpy(buf + n, dirent->d_name);
                    } else {
                        continue;
                    }
                    dobjects = processObject(dobjects, buf, NULL);
                }
                closedir(dir);
            } else {
                fprintf(out, "<p>Couldn't open directory: %s (%d).</p>\n",
                        strerror(errno), errno);
                goto trailer;
            }
        }
    }

    if(dobjects) {
        DiskObjectPtr dobject;
        int entryno;
        dobjects = insertRoot(dobjects, root);
        dobjects = insertDirs(dobjects);
        dobjects = filterDiskObjects(dobjects, root, recursive);
        dobject = dobjects;
        buf[0] = '\0';
        alternatingHttpStyle(out, "diskcachelist");
        fprintf(out, "<table id=diskcachelist>\n");
        fprintf(out, "<tbody>\n");
        entryno = 0;
        while(dobjects) {
            dobject = dobjects;
            i = strlen(dobject->location);
            isdir = (i == 0 || dobject->location[i - 1] == '/');
            if(entryno % 2)
                fprintf(out, "<tr class=odd>");
            else
                fprintf(out, "<tr class=even>");
            if(dobject->size >= 0) {
                fprintf(out, "<td><a href=\"%s\"><tt>",
                        dobject->location);
                htmlPrint(out,
                          dobject->location, strlen(dobject->location));
                fprintf(out, "</tt></a></td> ");
                if(dobject->length >= 0) {
                    if(dobject->size == dobject->length)
                        fprintf(out, "<td>%d</td> ", dobject->length);
                    else
                        fprintf(out, "<td>%d/%d</td> ",
                               dobject->size, dobject->length);
                } else {
                    /* Avoid a trigraph. */
                    fprintf(out, "<td>%d/<em>??" "?</em></td> ", dobject->size);
                }
                if(dobject->last_modified >= 0) {
                    struct tm *tm = gmtime(&dobject->last_modified);
                    if(tm == NULL)
                        n = -1;
                    else
                        n = strftime(buf, 1024, "%d.%m.%Y", tm);
                } else
                    n = -1;
                if(n > 0) {
                    buf[n] = '\0';
                    fprintf(out, "<td>%s</td> ", buf);
                } else {
                    fprintf(out, "<td></td>");
                }
                
                if(dobject->date >= 0) {
                    struct tm *tm = gmtime(&dobject->date);
                    if(tm == NULL)
                        n = -1;
                    else
                        n = strftime(buf, 1024, "%d.%m.%Y", tm);
                } else
                    n = -1;
                if(n > 0) {
                    buf[n] = '\0';
                    fprintf(out, "<td>%s</td>", buf);
                } else {
                    fprintf(out, "<td></td>");
                }
            } else {
                fprintf(out, "<td><tt>");
                htmlPrint(out, dobject->location,
                          strlen(dobject->location));
                fprintf(out, "</tt></td><td></td><td></td><td></td>");
            }
            if(isdir) {
                fprintf(out, "<td><a href=\"/polipo/index?%s\">plain</a></td>"
                        "<td><a href=\"/polipo/recursive-index?%s\">"
                        "recursive</a></td>",
                        dobject->location, dobject->location);
            }
            fprintf(out, "</tr>\n");
            entryno++;
            dobjects = dobject->next;
            free(dobject->location);
            free(dobject->filename);
            free(dobject);
        }
        fprintf(out, "</tbody>\n");
        fprintf(out, "</table>\n");
    }

 trailer:
    fprintf(out, "<p><a href=\"/polipo/\">back</a></p>\n");
    fprintf(out, "</body></html>\n");
    return;
}