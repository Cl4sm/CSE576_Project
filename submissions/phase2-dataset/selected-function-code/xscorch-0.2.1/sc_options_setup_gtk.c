
   sc_config_options *co = &w->c->options;
   sc_options_setup_data_gtk setup;
   ScDialog *dialog;
   int confirm = (SC_NETWORK_AUTH(w->c) ? SC_DIALOG_OK : 0);
   int row = 0;

   setup.c = w->c;
   setup.co = co;
   setup.modeidx  = 0;
   setup.teamidx  = 0;
   setup.orderidx = 0;
   setup.talkidx  = 0;
   setup.talkprob = co->talkprob;
   setup.extstatus= co->extstatus;
   setup.tooltips = co->tooltips;
   setup.interleave=co->interleave;
   while(sc_config_mode_types()[setup.modeidx] != co->mode)  ++setup.modeidx;
   while(sc_config_team_types()[setup.teamidx] != co->team)  ++setup.teamidx;
   while(sc_config_order_types()[setup.orderidx]!=co->order)++setup.orderidx;
   while(sc_config_talk_types()[setup.talkidx] != co->talk)  ++setup.talkidx;

   dialog = SC_DIALOG(sc_dialog_new("Options Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_options_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Mode",                 sc_link_combo_new(&setup.modeidx, sc_config_mode_names()), &row);
   attach_option(dialog, w, "N/A:  Teams",          sc_link_combo_new(&setup.teamidx, sc_config_team_names()), &row);
   attach_option(dialog, w, "Order",                sc_link_combo_new(&setup.orderidx, sc_config_order_names()), &row);
   attach_option(dialog, w, "Talk Mode",            sc_link_combo_new(&setup.talkidx, sc_config_talk_names()), &row);
   attach_option(dialog, w, "Talk Probability",     sc_link_spin_new(&setup.talkprob, 0, 100, 1), &row);
   attach_option(dialog, w, "Extended Status",      sc_link_check_new(&setup.extstatus), &row);
   attach_option(dialog, w, "Tooltips",             sc_link_check_new(&setup.tooltips), &row);
   attach_option(dialog, w, "Interleaved Tracking", sc_link_check_new(&setup.interleave), &row);
   
   sc_dialog_run(dialog);

}
