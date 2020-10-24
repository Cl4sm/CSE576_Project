static midend *midend_for_new_game(frontend *fe, const game *cgame,
                                   char *arg, int maybe_game_id,
                                   int maybe_save_file, char **error)
{
    midend *me = NULL;

    if (!arg) {
        if (me) midend_free(me);
        me = midend_new(fe, cgame, &win_drawing, fe);
        midend_new_game(me);
    } else {
        FILE *fp;
        char *err_param, *err_load;

        /*
         * See if arg is a valid filename of a save game file.
         */
        err_load = NULL;
        if (maybe_save_file && (fp = fopen(arg, "r")) != NULL) {
            const game *loadgame;

#ifdef COMBINED
            /*
             * Find out what kind of game is stored in the save
             * file; if we're going to end up loading that, it
             * will have to override our caller's judgment as to
             * what game to initialise our midend with.
             */
            char *id_name;
            err_load = identify_game(&id_name, savefile_read, fp);
            if (!err_load) {
                int i;
                for (i = 0; i < gamecount; i++)
                    if (!strcmp(id_name, gamelist[i]->name))
                        break;
                if (i == gamecount) {
                    err_load = "Save file is for a game not supported by"
                        " this program";
                } else {
                    loadgame = gamelist[i];
                    rewind(fp); /* go back to the start for actual load */
                }
            }
#else
            loadgame = cgame;
#endif
            if (!err_load) {
                if (me) midend_free(me);
                me = midend_new(fe, loadgame, &win_drawing, fe);
                err_load = midend_deserialise(me, savefile_read, fp);
            }
        } else {
            err_load = "Unable to open file";
        }

        if (maybe_game_id && (!maybe_save_file || err_load)) {
            /*
             * See if arg is a game description.
             */
            if (me) midend_free(me);
            me = midend_new(fe, cgame, &win_drawing, fe);
            err_param = midend_game_id(me, arg);
            if (!err_param) {
                midend_new_game(me);
            } else {
                if (maybe_save_file) {
                    *error = snewn(256 + strlen(arg) + strlen(err_param) +
                                   strlen(err_load), char);
                    sprintf(*error, "Supplied argument \"%s\" is neither a"
                            " game ID (%s) nor a save file (%s)",
                            arg, err_param, err_load);
                } else {
                    *error = dupstr(err_param);
                }
                midend_free(me);
                sfree(fe);
                return NULL;
            }
        } else if (err_load) {
            *error = dupstr(err_load);
            midend_free(me);
            sfree(fe);
            return NULL;
        }
    }

    return me;
}