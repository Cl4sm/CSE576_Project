void sc_physics_setup_gtk(sc_window_gtk *w) {

   sc_physics *ph = w->c->physics;
   sc_physics_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK : 0);
   int row = 0;

   setup.c = w->c;
   setup.ph = ph;
   setup.airviscosity = ph->airviscosity;
   setup.gravity      = ph->gravity;
   setup.damping      = ph->damping;
   setup.maxwind      = ph->maxwind;
   setup.dynamicwind  = ph->dynamicwind;
   setup.suspenddirt  = ph->suspenddirt;
   setup.tanksfall    = ph->tanksfall;
   setup.bordersextend= ph->bordersextend;
   setup.wallidx      = 0;
   while(ph->walls != sc_physics_wall_types()[setup.wallidx]) ++setup.wallidx;

   dialog = SC_DIALOG(sc_dialog_new("Physics Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_physics_setup_apply_gtk, &setup);

   attach_option(dialog, w, "N/A:  Air Viscosity", sc_link_spinf_new(&setup.airviscosity, 0, SC_PHYSICS_VISCOUS_MAX, 0.001), &row);
   attach_option(dialog, w, "Gravity",             sc_link_spinf_new(&setup.gravity, 0, SC_PHYSICS_GRAVITY_MAX, 0.01), &row);
   attach_option(dialog, w, "Ground Damping",      sc_link_spinf_new(&setup.damping, 0, SC_TRAJ_DAMPING_MAX, 0.01), &row);
   attach_option(dialog, w, "N/A:  Suspend Dirt",  sc_link_spin_new(&setup.suspenddirt, 0, 100, 1), &row);
   attach_option(dialog, w, "N/A:  Tanks Fall",    sc_link_spin_new(&setup.tanksfall, 0, 100, 1), &row);
   attach_option(dialog, w, "Borders Extend",      sc_link_spin_new(&setup.bordersextend, 0, SC_PHYSICS_BORDERS_MAX, SC_PHYSICS_BORDERS_MAX / 100), &row);
   attach_option(dialog, w, "Walls Are",           sc_link_combo_new(&setup.wallidx, sc_physics_wall_names()), &row);
   attach_option(dialog, w, "Maximum Wind Speed",  sc_link_spinf_new(&setup.maxwind, 0, SC_PHYSICS_WIND_MAX, 0.01), &row);
   attach_option(dialog, w, "Wind is Dynamic",     sc_link_check_new(&setup.dynamicwind), &row);

   sc_dialog_run(dialog);

}
