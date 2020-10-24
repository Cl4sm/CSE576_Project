void midend_free(midend *me)
{
    int i;

    midend_free_game(me);

    if (me->drawing)
	drawing_free(me->drawing);
    random_free(me->random);
    sfree(me->states);
    sfree(me->desc);
    sfree(me->privdesc);
    sfree(me->seedstr);
    sfree(me->aux_info);
    me->ourgame->free_params(me->params);
    if (me->npresets) {
	for (i = 0; i < me->npresets; i++) {
	    sfree(me->presets[i]);
	    sfree(me->preset_names[i]);
	    sfree(me->preset_encodings[i]);
	}
	sfree(me->presets);
	sfree(me->preset_names);
	sfree(me->preset_encodings);
    }
    if (me->ui)
        me->ourgame->free_ui(me->ui);
    if (me->curparams)
        me->ourgame->free_params(me->curparams);
    sfree(me->laststatus);
    sfree(me);
}