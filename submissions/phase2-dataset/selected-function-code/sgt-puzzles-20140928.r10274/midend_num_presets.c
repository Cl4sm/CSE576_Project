int midend_num_presets(midend *me)
{
    if (!me->npresets) {
        char *name;
        game_params *preset;

        while (me->ourgame->fetch_preset(me->npresets, &name, &preset)) {
            if (me->presetsize <= me->npresets) {
                me->presetsize = me->npresets + 10;
                me->presets = sresize(me->presets, me->presetsize,
                                      game_params *);
                me->preset_names = sresize(me->preset_names, me->presetsize,
                                           char *);
                me->preset_encodings = sresize(me->preset_encodings,
					       me->presetsize, char *);
            }

            me->presets[me->npresets] = preset;
            me->preset_names[me->npresets] = name;
            me->preset_encodings[me->npresets] =
		me->ourgame->encode_params(preset, TRUE);;
            me->npresets++;
        }
    }

    {
        /*
         * Allow environment-based extensions to the preset list by
         * defining a variable along the lines of `SOLO_PRESETS=2x3
         * Advanced:2x3da'. Colon-separated list of items,
         * alternating between textual titles in the menu and
         * encoded parameter strings.
         */
        char buf[80], *e, *p;
        int j, k;

        sprintf(buf, "%s_PRESETS", me->ourgame->name);
	for (j = k = 0; buf[j]; j++)
	    if (!isspace((unsigned char)buf[j]))
		buf[k++] = toupper((unsigned char)buf[j]);
	buf[k] = '\0';

        if ((e = getenv(buf)) != NULL) {
            p = e = dupstr(e);

            while (*p) {
                char *name, *val;
                game_params *preset;

                name = p;
                while (*p && *p != ':') p++;
                if (*p) *p++ = '\0';
                val = p;
                while (*p && *p != ':') p++;
                if (*p) *p++ = '\0';

                preset = me->ourgame->default_params();
                me->ourgame->decode_params(preset, val);

		if (me->ourgame->validate_params(preset, TRUE)) {
		    /* Drop this one from the list. */
		    me->ourgame->free_params(preset);
		    continue;
		}

                if (me->presetsize <= me->npresets) {
                    me->presetsize = me->npresets + 10;
                    me->presets = sresize(me->presets, me->presetsize,
                                          game_params *);
                    me->preset_names = sresize(me->preset_names,
                                               me->presetsize, char *);
                    me->preset_encodings = sresize(me->preset_encodings,
						   me->presetsize, char *);
                }

                me->presets[me->npresets] = preset;
                me->preset_names[me->npresets] = dupstr(name);
                me->preset_encodings[me->npresets] =
		    me->ourgame->encode_params(preset, TRUE);
                me->npresets++;
            }
            sfree(e);
        }
    }

    return me->npresets;
}