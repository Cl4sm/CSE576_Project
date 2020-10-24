
   sc_config_options *co = setup->co;
   sc_window_gtk *w = (sc_window_gtk *)setup->c->window;
   bool oldextstatus;
   bool oldtooltips;

   oldextstatus = co->extstatus;
   oldtooltips  = co->tooltips;
   co->mode     = sc_config_mode_types()[setup->modeidx];
   co->team     = sc_config_team_types()[setup->teamidx];
   co->order    = sc_config_order_types()[setup->orderidx];
   co->talk     = sc_config_talk_types()[setup->talkidx];
   co->talkprob = setup->talkprob;
   co->extstatus= setup->extstatus;
   co->tooltips = setup->tooltips;
   co->interleave=setup->interleave;
   
   /* Reconfigure the statusbar */
   if(oldextstatus != co->extstatus) sc_status_setup(setup->c->window);

   /* TEMP: I can't find a way to do this with GTK2...
    * Probably tooltips won't appear if they're off until you enable them and restart xscorch.
    *
   if(oldtooltips != co->tooltips) {
      if(co->tooltips) gtk_tooltips_enable(w);
      else             gtk_tooltips_disable(w);
   }
    */
      
   #if USE_NETWORK
   if(SC_NETWORK_SERVER(setup->c)) sc_net_server_send_config(setup->c, setup->c->server);
   #endif
   
}
