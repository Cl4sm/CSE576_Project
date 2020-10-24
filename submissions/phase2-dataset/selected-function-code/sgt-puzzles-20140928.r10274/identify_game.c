char *identify_game(char **name, int (*read)(void *ctx, void *buf, int len),
                    void *rctx)
{
    int nstates = 0, statepos = -1, gotstates = 0;
    int started = FALSE;

    char *val = NULL;
    /* Initially all errors give the same report */
    char *ret = "Data does not appear to be a saved game file";

    *name = NULL;

    /*
     * Loop round and round reading one key/value pair at a time from
     * the serialised stream, until we've found the game name.
     */
    while (nstates <= 0 || statepos < 0 || gotstates < nstates-1) {
        char key[9], c;
        int len;

        do {
            if (!read(rctx, key, 1)) {
                /* unexpected EOF */
                goto cleanup;
            }
        } while (key[0] == '\r' || key[0] == '\n');

        if (!read(rctx, key+1, 8)) {
            /* unexpected EOF */
            goto cleanup;
        }

        if (key[8] != ':') {
            if (started)
                ret = "Data was incorrectly formatted for a saved game file";
	    goto cleanup;
        }
        len = strcspn(key, ": ");
        assert(len <= 8);
        key[len] = '\0';

        len = 0;
        while (1) {
            if (!read(rctx, &c, 1)) {
                /* unexpected EOF */
                goto cleanup;
            }

            if (c == ':') {
                break;
            } else if (c >= '0' && c <= '9') {
                len = (len * 10) + (c - '0');
            } else {
                if (started)
                    ret = "Data was incorrectly formatted for a"
                    " saved game file";
                goto cleanup;
            }
        }

        val = snewn(len+1, char);
        if (!read(rctx, val, len)) {
            if (started)
            goto cleanup;
        }
        val[len] = '\0';

        if (!started) {
            if (strcmp(key, "SAVEFILE") || strcmp(val, SERIALISE_MAGIC)) {
                /* ret already has the right message in it */
                goto cleanup;
            }
            /* Now most errors are this one, unless otherwise specified */
            ret = "Saved data ended unexpectedly";
            started = TRUE;
        } else {
            if (!strcmp(key, "VERSION")) {
                if (strcmp(val, SERIALISE_VERSION)) {
                    ret = "Cannot handle this version of the saved game"
                        " file format";
                    goto cleanup;
                }
            } else if (!strcmp(key, "GAME")) {
                *name = dupstr(val);
                ret = NULL;
                goto cleanup;
            }
        }

        sfree(val);
        val = NULL;
    }

    cleanup:
    sfree(val);
    return ret;
}