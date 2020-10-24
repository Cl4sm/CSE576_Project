void sc_player_setup_gtk(sc_window_gtk *w) {

   char help1[SC_GTK_STRING_BUFFER];
   char help2[SC_GTK_STRING_BUFFER];
   char help3[SC_GTK_STRING_BUFFER];
   const char **tanklist;
   int tankcount;
   sc_config *c = w->c;
   sc_player_setup_data_gtk pl;
   ScDialog *dialog;
   GtkWidget *tbox;
   GtkWidget *widget;
   int confirm = (SC_NETWORK_MODE(c) ? 0 : SC_DIALOG_OK);
   unsigned int aitype;
   int i;
   
   /* What is the player minimum? */
   if(SC_NETWORK_MODE(c)) {
      pl.minplayers = 0;
      pl.aionly = true;
      for(i = 0; i < c->numplayers; ++i) {
         if(c->players[i]->aitype == SC_AI_HUMAN || c->players[i]->aitype == SC_AI_NETWORK) ++pl.minplayers;
      }
      pl.aitypes = sc_ai_types_nohuman();
      pl.ainames = sc_ai_names_nohuman();
   } else {
      pl.minplayers = 2;
      pl.aionly = false;
      pl.aitypes = sc_ai_types_nonet();
      pl.ainames = sc_ai_names_nonet();
   }
   
   pl.c = c;
   pl.numplayers = c->numplayers;
   pl.numrounds = c->numrounds;
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      strcopyb(pl.players[i].name, c->players[i]->name, sizeof(pl.players[i].name));
      pl.players[i].tankidx = sc_tank_profile_index_of(c->tanks, c->players[i]->tank);
      
      /* Compute the player index -- use caution! */
      aitype = c->players[i]->aitype;
      if(aitype == SC_AI_NETWORK) aitype = SC_AI_HUMAN;
      if(pl.aionly && aitype == SC_AI_HUMAN) {
         pl.players[i].enabled = false;
         pl.players[i].aiidx = 0;   /* caution: assumes human is first */
      } else {
         pl.players[i].enabled = true;
         pl.players[i].aiidx = 0;   /* caution: assumes human is first */
         while(pl.aitypes[pl.players[i].aiidx] != aitype) {
            ++pl.players[i].aiidx;
         }
      }
   }

   dialog = SC_DIALOG(sc_dialog_new("Player Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_player_setup_apply_gtk, &pl);

   tbox = gtk_table_new(2, 2, FALSE);
   gtk_table_attach(GTK_TABLE(dialog->grid), tbox, 0, 3, 0, 1, GTK_FILL, GTK_FILL | GTK_EXPAND, 2, 2);

   sc_help_text(help1, sizeof(help1), "Number of Players");
   widget = tooltip(w, help1, sc_label_new("Number of players:"));
   gtk_table_attach(GTK_TABLE(tbox), widget, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 2, 2);
   gtk_misc_set_alignment(GTK_MISC(widget), 0, 0.5);
   widget = tooltip(w, help1, sc_link_spin_new(&pl.numplayers, pl.minplayers, SC_MAX_PLAYERS, 1));
   g_signal_connect(G_OBJECT(widget), "modified",
                    (GCallback)_sc_player_sensitize_gtk, &pl);
   gtk_table_attach(GTK_TABLE(tbox), widget, 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 2, 2);

   sc_help_text(help1, sizeof(help1), "Number of Rounds");
   widget = tooltip(w, help1, sc_label_new("Number of rounds:"));
   gtk_table_attach(GTK_TABLE(tbox), widget, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 2, 2);
   gtk_misc_set_alignment(GTK_MISC(widget), 0, 0.5);
   widget = tooltip(w, help1, sc_link_spin_new(&pl.numrounds, 1, SC_MAX_ROUNDS, 1));
   g_signal_connect(G_OBJECT(widget), "modified",
                    (GCallback)_sc_player_sensitize_gtk, &pl);
   gtk_table_attach(GTK_TABLE(tbox), widget, 1, 2, 1, 2, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 2, 2);

   sc_help_text(help1, sizeof(help1), "AI Type");
   sc_help_text(help2, sizeof(help1), "Player Name");
   sc_help_text(help3, sizeof(help1), "Tank Style");
   sc_dialog_grid_attach(dialog, tooltip(w, help1, sc_label_new("AI type")),     3, 0);
   sc_dialog_grid_attach(dialog, tooltip(w, help2, sc_label_new("Player name")), 3, 1);
   sc_dialog_grid_attach(dialog, tooltip(w, help3, sc_label_new("Tank style")),  3, 2);
   
   tankcount = sc_tank_profile_size(c->tanks);
   tanklist = (const char **)malloc(sizeof(char *) * (tankcount + 1));
   for(i = 0; i < tankcount; ++i) {
      tanklist[i] = sc_tank_profile_lookup(c->tanks, i)->name;
   }
   tanklist[tankcount] = NULL;
   
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      if(pl.players[i].enabled) {
         pl.isai[i] = tooltip(w, help1, sc_link_combo_new(&pl.players[i].aiidx, pl.ainames));
      } else {
         pl.isai[i] = tooltip(w, help1, sc_link_combo_new(&pl.players[i].aiidx, sc_ai_names()));
      }
      g_signal_connect(G_OBJECT(pl.isai[i]), "modified",
                       (GCallback)_sc_player_sensitize_gtk, &pl);
      sc_dialog_grid_attach(dialog, pl.isai[i], i + 7, 0);

      pl.name[i] = tooltip(w, help2, sc_link_entry_new(pl.players[i].name, SC_PLAYER_NAME_LENGTH));
      g_signal_connect(G_OBJECT(pl.name[i]), "modified",
                       (GCallback)_sc_player_sensitize_gtk, &pl);
      sc_dialog_grid_attach(dialog, pl.name[i], i + 7, 1);
      
      pl.tank[i] = tooltip(w, help3, sc_link_combo_new(&pl.players[i].tankidx, tanklist));
      g_signal_connect(G_OBJECT(pl.tank[i]), "modified",
                       (GCallback)_sc_player_sensitize_gtk, &pl);
      sc_dialog_grid_attach(dialog, pl.tank[i], i + 7, 2);
   }
   
   _sc_player_sensitize_gtk(NULL, &pl);
   sc_dialog_run(dialog);
   
   free(tanklist);

}
