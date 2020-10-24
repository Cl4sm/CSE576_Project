void radeon_dump_img(char *type)
{
    int len = 0, i, ret;
    unsigned long location = 0;
    unsigned char * volatile fb;
    
    if (!radeon_get_fb_params(type, 0, &location, &len) || !len)
        die("mapping location to dump");

    fb = (unsigned char * volatile) location;

    i = 0;
    while (i < len) {
        ret = write(STDOUT_FILENO, &(fb[i]), len - i);
        if (ret < 0)
            die_error(errno, "writing to stdout");
        i += ret;
    }

    return;
}