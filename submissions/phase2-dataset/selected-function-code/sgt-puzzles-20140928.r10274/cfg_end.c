static void cfg_end(int use_results)
{
    if (use_results) {
        /*
         * User hit OK.
         */
        char *err = midend_set_config(me, cfg_which, cfg);

        if (err) {
            /*
             * The settings were unacceptable, so leave the config box
             * open for the user to adjust them and try again.
             */
            js_error_box(err);
        } else {
            /*
             * New settings are fine; start a new game and close the
             * dialog.
             */
            select_appropriate_preset();
            midend_new_game(me);
            resize();
            midend_redraw(me);
            free_cfg(cfg);
            js_dialog_cleanup();
        }
    } else {
        /*
         * User hit Cancel. Close the dialog, but also we must still
         * reselect the right element of the dropdown list.
         *
         * (Because: imagine you have a preset selected, and then you
         * select Custom from the list, but change your mind and hit
         * Esc. The Custom option will now still be selected in the
         * list, whereas obviously it should show the preset you still
         * _actually_ have selected. Worse still, it'll be the visible
         * rather than invisible Custom option - see the comment in
         * js_add_preset in emcclib.js - so you won't even be able to
         * select Custom without a faffy workaround.)
         */
        select_appropriate_preset();

        free_cfg(cfg);
        js_dialog_cleanup();
    }
}