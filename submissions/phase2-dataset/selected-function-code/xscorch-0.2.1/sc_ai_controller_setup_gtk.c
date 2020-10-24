void sc_ai_controller_setup_gtk(sc_window_gtk *w) {

   sc_ai_controller *aic = w->c->aicontrol;
   sc_ai_controller_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK : 0);
   int row = 0;

   setup.c = w->c;
   setup.aic = aic;
   setup.humantargets = aic->humantargets;
   setup.allowoffsets = aic->allowoffsets;
   setup.alwaysoffset = aic->alwaysoffset;
   setup.enablescan = aic->enablescan;
   setup.nobudget = aic->nobudget;

   dialog = SC_DIALOG(sc_dialog_new("AI Controller Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_ai_controller_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Human Target Practice",  sc_link_check_new(&setup.humantargets), &row);
   attach_option(dialog, w, "Allow Offset Targetting",sc_link_check_new(&setup.allowoffsets), &row);
   attach_option(dialog, w, "Always Offset",          sc_link_check_new(&setup.alwaysoffset), &row);
   attach_option(dialog, w, "Enable Scan Refinement", sc_link_check_new(&setup.enablescan), &row);
   attach_option(dialog, w, "No Budget Constraints",  sc_link_check_new(&setup.nobudget), &row);

   sc_dialog_run(dialog);

}
