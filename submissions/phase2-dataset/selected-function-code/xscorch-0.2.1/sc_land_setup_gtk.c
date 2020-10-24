
   sc_land *land = w->c->land;
   sc_land_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK | SC_DIALOG_APPLY : 0);
   int row = 0;

   setup.c = w->c;
   setup.w = w;
   setup.land = land;
   setup.hostileenv  = land->hostileenv;
   setup.generatoridx= 0;
   setup.bumpiness   = land->bumpiness;
   setup.skyidx      = 0;
   while(land->sky != sc_land_sky_types()[setup.skyidx]) ++setup.skyidx;
   while(land->generator != sc_land_generator_types()[setup.generatoridx]) ++setup.generatoridx;

   dialog = SC_DIALOG(sc_dialog_new("Landscape Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_land_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Land Generator",            sc_link_combo_new(&setup.generatoridx, sc_land_generator_names()), &row);
   attach_option(dialog, w, "Bumpiness",                 sc_link_spinf_new(&setup.bumpiness, 0, SC_LAND_BUMPINESS_MAX, 0.1), &row);
   attach_option(dialog, w, "Sky",                       sc_link_combo_new(&setup.skyidx, sc_land_sky_names()), &row);
   attach_option(dialog, w, "N/A:  Hostile Environment", sc_link_check_new(&setup.hostileenv), &row);

   sc_dialog_run(dialog);

}
