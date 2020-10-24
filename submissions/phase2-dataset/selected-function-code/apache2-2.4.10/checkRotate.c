static void checkRotate(rotate_config_t *config, rotate_status_t *status)
{
    if (status->current.fd == NULL) {
        status->rotateReason = ROTATE_NEW;
    }
    else if (config->sRotation) {
        apr_finfo_t finfo;
        apr_off_t current_size = -1;

        if (apr_file_info_get(&finfo, APR_FINFO_SIZE, status->current.fd) == APR_SUCCESS) {
            current_size = finfo.size;
        }

        if (current_size > config->sRotation) {
            status->rotateReason = ROTATE_SIZE;
        }
        else if (config->tRotation) {
            if (get_now(config) >= status->tLogEnd) {
                status->rotateReason = ROTATE_TIME;
            }
        }
    }
    else if (config->tRotation) {
        if (get_now(config) >= status->tLogEnd) {
            status->rotateReason = ROTATE_TIME;
        }
    }
    else {
        fprintf(stderr, "No rotation time or size specified\n");
        exit(2);
    }

    if (status->rotateReason != ROTATE_NONE && config->verbose) {
        fprintf(stderr, "File rotation needed, reason: %s\n", ROTATE_REASONS[status->rotateReason]);
    }
}