void sc_weapons_setup_gtk(sc_window_gtk *w) {

   sc_weapon_config *wpc = w->c->weapons;
   sc_weapons_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK : 0);
   int row = 0;

   setup.c = w->c;
   setup.wpc = wpc;
   setup.armslevel   = wpc->armslevel;
   setup.bombicon    = wpc->bombiconsize;
   setup.tunneling   = wpc->tunneling;
   setup.scaling     = wpc->scaling;
   setup.tracepaths  = wpc->tracepaths;
   setup.uselessitems= wpc->uselessitems;

   dialog = SC_DIALOG(sc_dialog_new("Weapon Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_weapons_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Arms Level",    sc_link_spin_new(&setup.armslevel, 0, SC_ARMS_LEVEL_MAX, 1), &row);
   attach_option(dialog, w, "Bomb Icon Size",sc_link_spin_new(&setup.bombicon, 0, SC_WEAPON_BOMB_ICON_MAX, 1), &row);
   attach_option(dialog, w, "Scaling",       sc_link_spinf_new(&setup.scaling, 0, SC_WEAPON_SCALING_MAX, 0.01), &row);
   attach_option(dialog, w, "Tunneling",     sc_link_check_new(&setup.tunneling), &row);
   attach_option(dialog, w, "Trace Paths",   sc_link_check_new(&setup.tracepaths), &row);
   attach_option(dialog, w, "Useless Items", sc_link_check_new(&setup.uselessitems), &row);

   sc_dialog_run(dialog);

}
