TestGIF(char *file)
{
    FILE *fd = fopen(file, "r");
    char buf[10];
    int ret = 0;

    if (fd == NULL)
	return 0;

    if (ReadOK(fd, buf, 6)) {
	if ((strncmp(buf, "GIF", 3) == 0) &&
	    ((strcmp(buf + 3, "87a") != 0) ||
	     (strcmp(buf + 3, "89a") != 0)))
	    ret = 1;
    }
    fclose(fd);

    return ret;
}
