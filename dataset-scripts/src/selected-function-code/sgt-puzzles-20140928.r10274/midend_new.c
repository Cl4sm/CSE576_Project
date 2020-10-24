midend *midend_new(frontend *fe, const game *ourgame,
		   const drawing_api *drapi, void *drhandle)
{
    midend *me = snew(midend);
    void *randseed;
    int randseedsize;

    get_random_seed(&randseed, &randseedsize);

    me->frontend = fe;
    me->ourgame = ourgame;
    me->random = random_new(randseed, randseedsize);
    me->nstates = me->statesize = me->statepos = 0;
    me->states = NULL;
    me->params = ourgame->default_params();
    me->game_id_change_notify_function = NULL;
    me->game_id_change_notify_ctx = NULL;

    /*
     * Allow environment-based changing of the default settings by
     * defining a variable along the lines of `NET_DEFAULT=25x25w'
     * in which the value is an encoded parameter string.
     */
    {
        char buf[80], *e;
        int j, k;
        sprintf(buf, "%s_DEFAULT", me->ourgame->name);
	for (j = k = 0; buf[j]; j++)
	    if (!isspace((unsigned char)buf[j]))
		buf[k++] = toupper((unsigned char)buf[j]);
	buf[k] = '\0';
        if ((e = getenv(buf)) != NULL)
            me->ourgame->decode_params(me->params, e);
    }
    me->curparams = NULL;
    me->desc = me->privdesc = NULL;
    me->seedstr = NULL;
    me->aux_info = NULL;
    me->genmode = GOT_NOTHING;
    me->drawstate = NULL;
    me->oldstate = NULL;
    me->presets = NULL;
    me->preset_names = NULL;
    me->preset_encodings = NULL;
    me->npresets = me->presetsize = 0;
    me->anim_time = me->anim_pos = 0.0F;
    me->flash_time = me->flash_pos = 0.0F;
    me->dir = 0;
    me->ui = NULL;
    me->pressed_mouse_button = 0;
    me->laststatus = NULL;
    me->timing = FALSE;
    me->elapsed = 0.0F;
    me->tilesize = me->winwidth = me->winheight = 0;
    if (drapi)
	me->drawing = drawing_new(drapi, me, drhandle);
    else
	me->drawing = NULL;

    midend_reset_tilesize(me);

    sfree(randseed);

    return me;
}