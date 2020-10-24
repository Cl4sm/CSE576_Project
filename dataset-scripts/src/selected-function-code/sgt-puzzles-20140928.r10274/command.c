void command(int n)
{
    switch (n) {
      case 0:                          /* specific game ID */
        cfg_start(CFG_DESC);
        break;
      case 1:                          /* random game seed */
        cfg_start(CFG_SEED);
        break;
      case 2:                          /* game parameter dropdown changed */
        {
            int i = js_get_selected_preset();
            if (i < 0) {
                /*
                 * The user selected 'Custom', so launch the config
                 * box.
                 */
                if (thegame.can_configure) /* (double-check just in case) */
                    cfg_start(CFG_SETTINGS);
            } else {
                /*
                 * The user selected a preset, so just switch straight
                 * to that.
                 */
                assert(i < npresets);
                midend_set_params(me, presets[i]);
                midend_new_game(me);
                resize();
                midend_redraw(me);
                update_undo_redo();
                js_focus_canvas();
                select_appropriate_preset(); /* sort out Custom/Customise */
            }
        }
        break;
      case 3:                          /* OK clicked in a config box */
        cfg_end(TRUE);
        update_undo_redo();
        break;
      case 4:                          /* Cancel clicked in a config box */
        cfg_end(FALSE);
        update_undo_redo();
        break;
      case 5:                          /* New Game */
        midend_process_key(me, 0, 0, 'n');
        update_undo_redo();
        js_focus_canvas();
        break;
      case 6:                          /* Restart */
        midend_restart_game(me);
        update_undo_redo();
        js_focus_canvas();
        break;
      case 7:                          /* Undo */
        midend_process_key(me, 0, 0, 'u');
        update_undo_redo();
        js_focus_canvas();
        break;
      case 8:                          /* Redo */
        midend_process_key(me, 0, 0, 'r');
        update_undo_redo();
        js_focus_canvas();
        break;
      case 9:                          /* Solve */
        if (thegame.can_solve) {
            char *msg = midend_solve(me);
            if (msg)
                js_error_box(msg);
        }
        update_undo_redo();
        js_focus_canvas();
        break;
    }
}