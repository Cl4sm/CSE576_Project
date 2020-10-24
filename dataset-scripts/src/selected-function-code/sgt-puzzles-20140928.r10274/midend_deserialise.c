char *midend_deserialise(midend *me,
                         int (*read)(void *ctx, void *buf, int len),
                         void *rctx)
{
    int nstates = 0, statepos = -1, gotstates = 0;
    int started = FALSE;
    int i;

    char *val = NULL;
    /* Initially all errors give the same report */
    char *ret = "Data does not appear to be a saved game file";

    /*
     * We construct all the new state in local variables while we
     * check its sanity. Only once we have finished reading the
     * serialised data and detected no errors at all do we start
     * modifying stuff in the midend passed in.
     */
    char *seed = NULL, *parstr = NULL, *desc = NULL, *privdesc = NULL;
    char *auxinfo = NULL, *uistr = NULL, *cparstr = NULL;
    float elapsed = 0.0F;
    game_params *params = NULL, *cparams = NULL;
    game_ui *ui = NULL;
    struct midend_state_entry *states = NULL;

    /*
     * Loop round and round reading one key/value pair at a time
     * from the serialised stream, until we have enough game states
     * to finish.
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
                if (strcmp(val, me->ourgame->name)) {
                    ret = "Save file is from a different game";
                    goto cleanup;
                }
            } else if (!strcmp(key, "PARAMS")) {
                sfree(parstr);
                parstr = val;
                val = NULL;
            } else if (!strcmp(key, "CPARAMS")) {
                sfree(cparstr);
                cparstr = val;
                val = NULL;
            } else if (!strcmp(key, "SEED")) {
                sfree(seed);
                seed = val;
                val = NULL;
            } else if (!strcmp(key, "DESC")) {
                sfree(desc);
                desc = val;
                val = NULL;
            } else if (!strcmp(key, "PRIVDESC")) {
                sfree(privdesc);
                privdesc = val;
                val = NULL;
            } else if (!strcmp(key, "AUXINFO")) {
                unsigned char *tmp;
                int len = strlen(val) / 2;   /* length in bytes */
                tmp = hex2bin(val, len);
                obfuscate_bitmap(tmp, len*8, TRUE);

                sfree(auxinfo);
                auxinfo = snewn(len + 1, char);
                memcpy(auxinfo, tmp, len);
                auxinfo[len] = '\0';
                sfree(tmp);
            } else if (!strcmp(key, "UI")) {
                sfree(uistr);
                uistr = val;
                val = NULL;
            } else if (!strcmp(key, "TIME")) {
                elapsed = (float)atof(val);
            } else if (!strcmp(key, "NSTATES")) {
                nstates = atoi(val);
                if (nstates <= 0) {
                    ret = "Number of states in save file was negative";
                    goto cleanup;
                }
                if (states) {
                    ret = "Two state counts provided in save file";
                    goto cleanup;
                }
                states = snewn(nstates, struct midend_state_entry);
                for (i = 0; i < nstates; i++) {
                    states[i].state = NULL;
                    states[i].movestr = NULL;
                    states[i].movetype = NEWGAME;
                }
            } else if (!strcmp(key, "STATEPOS")) {
                statepos = atoi(val);
            } else if (!strcmp(key, "MOVE")) {
                gotstates++;
                states[gotstates].movetype = MOVE;
                states[gotstates].movestr = val;
                val = NULL;
            } else if (!strcmp(key, "SOLVE")) {
                gotstates++;
                states[gotstates].movetype = SOLVE;
                states[gotstates].movestr = val;
                val = NULL;
            } else if (!strcmp(key, "RESTART")) {
                gotstates++;
                states[gotstates].movetype = RESTART;
                states[gotstates].movestr = val;
                val = NULL;
            }
        }

        sfree(val);
        val = NULL;
    }

    params = me->ourgame->default_params();
    me->ourgame->decode_params(params, parstr);
    if (me->ourgame->validate_params(params, TRUE)) {
        ret = "Long-term parameters in save file are invalid";
        goto cleanup;
    }
    cparams = me->ourgame->default_params();
    me->ourgame->decode_params(cparams, cparstr);
    if (me->ourgame->validate_params(cparams, FALSE)) {
        ret = "Short-term parameters in save file are invalid";
        goto cleanup;
    }
    if (seed && me->ourgame->validate_params(cparams, TRUE)) {
        /*
         * The seed's no use with this version, but we can perfectly
         * well use the rest of the data.
         */
        sfree(seed);
        seed = NULL;
    }
    if (!desc) {
        ret = "Game description in save file is missing";
        goto cleanup;
    } else if (me->ourgame->validate_desc(params, desc)) {
        ret = "Game description in save file is invalid";
        goto cleanup;
    }
    if (privdesc && me->ourgame->validate_desc(params, privdesc)) {
        ret = "Game private description in save file is invalid";
        goto cleanup;
    }
    if (statepos < 0 || statepos >= nstates) {
        ret = "Game position in save file is out of range";
    }

    states[0].state = me->ourgame->new_game(me, params,
                                            privdesc ? privdesc : desc);
    for (i = 1; i < nstates; i++) {
        assert(states[i].movetype != NEWGAME);
        switch (states[i].movetype) {
          case MOVE:
          case SOLVE:
            states[i].state = me->ourgame->execute_move(states[i-1].state,
                                                        states[i].movestr);
            if (states[i].state == NULL) {
                ret = "Save file contained an invalid move";
                goto cleanup;
            }
            break;
          case RESTART:
            if (me->ourgame->validate_desc(params, states[i].movestr)) {
                ret = "Save file contained an invalid restart move";
                goto cleanup;
            }
            states[i].state = me->ourgame->new_game(me, params,
                                                    states[i].movestr);
            break;
        }
    }

    ui = me->ourgame->new_ui(states[0].state);
    me->ourgame->decode_ui(ui, uistr);

    /*
     * Now we've run out of possible error conditions, so we're
     * ready to start overwriting the real data in the current
     * midend. We'll do this by swapping things with the local
     * variables, so that the same cleanup code will free the old
     * stuff.
     */
    {
        char *tmp;

        tmp = me->desc;
        me->desc = desc;
        desc = tmp;

        tmp = me->privdesc;
        me->privdesc = privdesc;
        privdesc = tmp;

        tmp = me->seedstr;
        me->seedstr = seed;
        seed = tmp;

        tmp = me->aux_info;
        me->aux_info = auxinfo;
        auxinfo = tmp;
    }

    me->genmode = GOT_NOTHING;

    me->statesize = nstates;
    nstates = me->nstates;
    me->nstates = me->statesize;
    {
        struct midend_state_entry *tmp;
        tmp = me->states;
        me->states = states;
        states = tmp;
    }
    me->statepos = statepos;

    {
        game_params *tmp;

        tmp = me->params;
        me->params = params;
        params = tmp;

        tmp = me->curparams;
        me->curparams = cparams;
        cparams = tmp;
    }

    me->oldstate = NULL;
    me->anim_time = me->anim_pos = me->flash_time = me->flash_pos = 0.0F;
    me->dir = 0;

    {
        game_ui *tmp;

        tmp = me->ui;
        me->ui = ui;
        ui = tmp;
    }

    me->elapsed = elapsed;
    me->pressed_mouse_button = 0;

    midend_set_timer(me);

    if (me->drawstate)
        me->ourgame->free_drawstate(me->drawing, me->drawstate);
    me->drawstate =
        me->ourgame->new_drawstate(me->drawing,
				   me->states[me->statepos-1].state);
    midend_size_new_drawstate(me);

    ret = NULL;                        /* success! */

    cleanup:
    sfree(val);
    sfree(seed);
    sfree(parstr);
    sfree(cparstr);
    sfree(desc);
    sfree(privdesc);
    sfree(auxinfo);
    sfree(uistr);
    if (params)
        me->ourgame->free_params(params);
    if (cparams)
        me->ourgame->free_params(cparams);
    if (ui)
        me->ourgame->free_ui(ui);
    if (states) {
        int i;

        for (i = 0; i < nstates; i++) {
            if (states[i].state)
                me->ourgame->free_game(states[i].state);
            sfree(states[i].movestr);
        }
        sfree(states);
    }

    return ret;
}