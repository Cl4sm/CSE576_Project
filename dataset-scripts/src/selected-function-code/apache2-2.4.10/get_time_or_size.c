static const char *get_time_or_size(rotate_config_t *config,
                                    const char *arg, int last) {
    char *ptr = NULL;
    /* Byte multiplier */
    unsigned int mult = 1;
    if ((ptr = strchr(arg, 'B')) != NULL) { /* Found KB size */
        mult = 1;
    }
    else if ((ptr = strchr(arg, 'K')) != NULL) { /* Found KB size */
        mult = 1024;
    }
    else if ((ptr = strchr(arg, 'M')) != NULL) { /* Found MB size */
        mult = 1024 * 1024;
    }
    else if ((ptr = strchr(arg, 'G')) != NULL) { /* Found GB size */
        mult = 1024 * 1024 * 1024;
    }
    if (ptr) { /* rotation based on file size */
        if (config->sRotation > 0) {
            return "Rotation size parameter allowed only once";
        }
        if (*(ptr+1) == '\0') {
            config->sRotation = atoi(arg) * mult;
        }
        if (config->sRotation == 0) {
            return "Invalid rotation size parameter";
        }
    }
    else if ((config->sRotation > 0 || config->tRotation > 0) && last) {
        /* rotation based on elapsed time */
        if (config->use_localtime) {
            return "UTC offset parameter is not valid with -l";
        }
        config->utc_offset = atoi(arg) * 60;
    }
    else { /* rotation based on elapsed time */
        if (config->tRotation > 0) {
            return "Rotation time parameter allowed only once";
        }
        config->tRotation = atoi(arg);
        if (config->tRotation <= 0) {
            return "Invalid rotation time parameter";
        }
    }
    return NULL;
}