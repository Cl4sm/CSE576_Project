SDlg *sdlg_create( SDL_Surface *list_frame, SDL_Surface *list_buttons,
                   int list_button_w, int list_button_h, int cell_h,
                   SDL_Surface *ctrl_frame, SDL_Surface *ctrl_buttons,
                   int ctrl_button_w, int ctrl_button_h, int id_ctrl,
                   SDL_Surface *mod_frame, SDL_Surface *mod_buttons,
                   int mod_button_w, int mod_button_h, int id_mod,
                   SDL_Surface *conf_frame, SDL_Surface *conf_buttons,
                   int conf_button_w, int conf_button_h, int id_conf,
                   Label *label,
                   void (*list_render_cb)(void*,SDL_Surface*),
                   void (*list_select_cb)(void*),
                   SDL_Surface *surf, int x, int y )
{
    int border = 10, alpha = 160, px, py;
    int cell_w = list_frame->w - 2 * border;
    int cell_count = ( list_frame->h - 2 * border ) / ( cell_h + 1 );

    SDlg *sdlg = calloc( 1, sizeof( SDlg ) );

    /* listbox for players */
    if ( ( sdlg->list = lbox_create( list_frame, alpha, border, list_buttons, list_button_w, list_button_h, label,
                                     cell_count, 2, cell_w, cell_h, 1, 0x0000ff, list_render_cb, surf, x, y ) ) == 0 )
        goto failure;

    /* group with human/cpu control button */
    if ( ( sdlg->ctrl = group_create( ctrl_frame, alpha, ctrl_buttons, 
                                      ctrl_button_w, ctrl_button_h, 1, id_ctrl, label, surf, x + list_frame->w - 1, y ) ) == 0 )
        goto failure;
    group_add_button( sdlg->ctrl, id_ctrl, ctrl_frame->w - border - ctrl_button_w, ( ctrl_frame->h - ctrl_button_h ) / 2, 0, tr("Switch Control") );

    /* group with ai module select button */
    if ( ( sdlg->module = group_create( mod_frame, alpha, mod_buttons, 
                                        mod_button_w, mod_button_h, 1, id_mod, label, surf, 
                                        x + list_frame->w - 1, y + ctrl_frame->h ) ) == 0 )
        goto failure;
    group_add_button( sdlg->module, id_mod, ctrl_frame->w - border - ctrl_button_w, ( mod_frame->h - mod_button_h ) / 2, 0, tr("Select AI Module") );
#ifndef USE_DL
    group_set_active( sdlg->module, id_mod, 0 );
#endif

    /* group with settings and confirm buttons; id_conf is id of first button
     * in image conf_buttons */
    if ( ( sdlg->confirm = group_create( conf_frame, alpha, conf_buttons, 
                                         conf_button_w, conf_button_h, 6, id_conf, label, surf, 
                                         x + list_frame->w - 1, y + ctrl_frame->h + mod_frame->h ) ) == 0 )
        goto failure;
    px = conf_frame->w - (border + conf_button_w);
    py = (conf_frame->h - conf_button_h) / 2;
    group_add_button( sdlg->confirm, ID_SETUP_OK, px, py, 0, tr("Ok") );
    px = border;
    group_add_button( sdlg->confirm, ID_SETUP_FOG, px, py, 1, tr("Fog Of War") );
    px += border + conf_button_w;
    group_add_button( sdlg->confirm, ID_SETUP_SUPPLY, px, py, 1, tr("Unit Supply") );
    px += border + conf_button_w;
    group_add_button( sdlg->confirm, ID_SETUP_WEATHER, px, py, 1, tr("Weather Influence") );
    px += border + conf_button_w;
    group_add_button( sdlg->confirm, ID_SETUP_DEPLOYTURN, px, py, 1, tr("Deploy Turn") );
    px += border + conf_button_w;
    group_add_button( sdlg->confirm, ID_SETUP_PURCHASE, px, py, 1, tr("Purchase Option") );
    group_lock_button( sdlg->confirm, ID_SETUP_FOG, config.fog_of_war );
    group_lock_button( sdlg->confirm, ID_SETUP_SUPPLY, config.supply );
    group_lock_button( sdlg->confirm, ID_SETUP_WEATHER, config.weather );
    group_lock_button( sdlg->confirm, ID_SETUP_DEPLOYTURN, config.deploy_turn );
    group_lock_button( sdlg->confirm, ID_SETUP_PURCHASE, config.purchase );
    sdlg->select_cb = list_select_cb;
    return sdlg;
failure:
    sdlg_delete( &sdlg );
    return 0;
}