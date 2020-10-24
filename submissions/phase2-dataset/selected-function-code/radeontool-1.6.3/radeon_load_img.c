void radeon_load_img(char *type)
{
    int len = 0, i, ret;
    unsigned long location = 0;
    unsigned char * volatile fb;
    
    if (!radeon_get_fb_params(type, 1, &location, &len))
        die("mapping framebuffer to load");

    if (!len)
        die("mapping framebuffer to load");
    fb = (unsigned char * volatile) location;

    i = 0;
    while (i < len) {
        ret = read(STDIN_FILENO, &(fb[i]), len - i);
        if (ret < 0)
            die_error(errno, "reading from stdin");
        i += ret;
    }

    return;
}