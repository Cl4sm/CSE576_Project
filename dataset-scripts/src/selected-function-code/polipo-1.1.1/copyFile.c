static int
copyFile(int from, char *filename, int n)
{
    char *buf;
    int to, offset, nread, nzeroes, rc;

    buf = malloc(CHUNK_SIZE);
    if(buf == NULL)
        return -1;

    to = open(filename, O_RDWR | O_CREAT | O_EXCL | O_BINARY,
	      diskCacheFilePermissions);
    if(to < 0) {
        free(buf);
        return -1;
    }

    offset = 0;
    while(offset < n) {
        nread = read(from, buf, MIN(CHUNK_SIZE, n - offset));
        if(nread <= 0)
            break;
        nzeroes = checkForZeroes(buf, nread & -8);
        if(nzeroes > 0) {
            /* I like holes */
            rc = lseek(to, nzeroes, SEEK_CUR);
            if(rc != offset + nzeroes) {
                if(rc < 0)
                    do_log_error(L_ERROR, errno, "Couldn't extend file");
                else
                    do_log(L_ERROR, 
                           "Couldn't extend file: "
                           "unexpected offset %d != %d + %d.\n",
                           rc, offset, nread);
                break;
            }
        }
        if(nread > nzeroes) {
            rc = write(to, buf + nzeroes, nread - nzeroes);
            if(rc != nread - nzeroes) {
                if(rc < 0)
                    do_log_error(L_ERROR, errno, "Couldn't write");
                else
                    do_log(L_ERROR, "Short write.\n");
                break;
            }
        }
        offset += nread;
    }
    free(buf);
    close(to);
    if(offset <= 0)
        unlink(filename);       /* something went wrong straight away */
    return 1;
}