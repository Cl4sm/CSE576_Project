
   sc_economy_config *ec = w->c->economics;
   sc_economy_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK : 0);
   int row = 0;

   setup.c = w->c;
   setup.ec = ec;
   setup.interestrate   = ec->interestrate;
   setup.dynamicinterest= ec->dynamicinterest;
   setup.initialcash    = ec->initialcash;
   setup.computersbuy   = ec->computersbuy;
   setup.computersaggressive = ec->computersaggressive;
   setup.freemarket     = ec->freemarket;
   setup.lottery        = ec->lottery;
   setup.economyidx     = _sc_economy_init_names_gtk(&setup);

   dialog = SC_DIALOG(sc_dialog_new("Economy Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_economy_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Scoring",              sc_link_combo_new(&setup.economyidx, setup.enames), &row);
   attach_option(dialog, w, "Initial Cash",         sc_link_spin_new(&setup.initialcash, 0, SC_ECONOMY_MAX_CASH, SC_ECONOMY_MAX_CASH / 100), &row);
   attach_option(dialog, w, "Interest Rate",        sc_link_spinf_new(&setup.interestrate, 0, SC_ECONOMY_MAX_INTEREST, 0.01), &row);
   attach_option(dialog, w, "Dynamic Interest",     sc_link_check_new(&setup.dynamicinterest), &row);
   attach_option(dialog, w, "AIs Can Buy",          sc_link_check_new(&setup.computersbuy), &row);
   attach_option(dialog, w, "AIs Buy Aggressively", sc_link_check_new(&setup.computersaggressive), &row);
   attach_option(dialog, w, "N/A:  Free Market",    sc_link_check_new(&setup.freemarket), &row);
   attach_option(dialog, w, "The Lottery",          sc_link_check_new(&setup.lottery), &row);

   sc_dialog_run(dialog);

}
