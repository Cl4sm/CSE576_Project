static char *midend_game_id_int(midend *me, char *id, int defmode)
{
    char *error, *par, *desc, *seed;
    game_params *newcurparams, *newparams, *oldparams1, *oldparams2;
    int free_params;

    seed = strchr(id, '#');
    desc = strchr(id, ':');

    if (desc && (!seed || desc < seed)) {
        /*
         * We have a colon separating parameters from game
         * description. So `par' now points to the parameters
         * string, and `desc' to the description string.
         */
        *desc++ = '\0';
        par = id;
        seed = NULL;
    } else if (seed && (!desc || seed < desc)) {
        /*
         * We have a hash separating parameters from random seed.
         * So `par' now points to the parameters string, and `seed'
         * to the seed string.
         */
        *seed++ = '\0';
        par = id;
        desc = NULL;
    } else {
        /*
         * We only have one string. Depending on `defmode', we take
         * it to be either parameters, seed or description.
         */
        if (defmode == DEF_SEED) {
            seed = id;
            par = desc = NULL;
        } else if (defmode == DEF_DESC) {
            desc = id;
            par = seed = NULL;
        } else {
            par = id;
            seed = desc = NULL;
        }
    }

    /*
     * We must be reasonably careful here not to modify anything in
     * `me' until we have finished validating things. This function
     * must either return an error and do nothing to the midend, or
     * return success and do everything; nothing in between is
     * acceptable.
     */
    newcurparams = newparams = oldparams1 = oldparams2 = NULL;

    if (par) {
        newcurparams = me->ourgame->dup_params(me->params);
        me->ourgame->decode_params(newcurparams, par);
        error = me->ourgame->validate_params(newcurparams, desc == NULL);
        if (error) {
            me->ourgame->free_params(newcurparams);
            return error;
        }
        oldparams1 = me->curparams;

        /*
         * Now filter only the persistent parts of this state into
         * the long-term params structure, unless we've _only_
         * received a params string in which case the whole lot is
         * persistent.
         */
        oldparams2 = me->params;
        if (seed || desc) {
            char *tmpstr;

            newparams = me->ourgame->dup_params(me->params);

            tmpstr = me->ourgame->encode_params(newcurparams, FALSE);
            me->ourgame->decode_params(newparams, tmpstr);

            sfree(tmpstr);
        } else {
            newparams = me->ourgame->dup_params(newcurparams);
        }
        free_params = TRUE;
    } else {
        newcurparams = me->curparams;
        newparams = me->params;
        free_params = FALSE;
    }

    if (desc) {
        error = me->ourgame->validate_desc(newparams, desc);
        if (error) {
            if (free_params) {
                if (newcurparams)
                    me->ourgame->free_params(newcurparams);
                if (newparams)
                    me->ourgame->free_params(newparams);
            }
            return error;
        }
    }

    /*
     * Now we've got past all possible error points. Update the
     * midend itself.
     */
    me->params = newparams;
    me->curparams = newcurparams;
    if (oldparams1)
        me->ourgame->free_params(oldparams1);
    if (oldparams2)
        me->ourgame->free_params(oldparams2);

    sfree(me->desc);
    sfree(me->privdesc);
    me->desc = me->privdesc = NULL;
    sfree(me->seedstr);
    me->seedstr = NULL;

    if (desc) {
        me->desc = dupstr(desc);
        me->genmode = GOT_DESC;
        sfree(me->aux_info);
	me->aux_info = NULL;
    }

    if (seed) {
        me->seedstr = dupstr(seed);
        me->genmode = GOT_SEED;
    }

    return NULL;
}