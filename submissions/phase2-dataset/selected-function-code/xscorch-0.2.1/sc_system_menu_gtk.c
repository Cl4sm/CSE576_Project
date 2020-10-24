void sc_system_menu_gtk(sc_window_gtk *w) {

   char help[SC_GTK_STRING_BUFFER];
   sc_config *c = w->c;
   sc_system_menu_data_gtk setup;
   GtkWidget *button;
   ScDialog *dialog;
   int row = 0;

   setup.c = c;
   setup.gfxanimate     = c->graphics.gfxanimate;
   setup.gfxfast        = c->graphics.gfxfast;
   setup.gfxcompfast    = c->graphics.gfxcompfast;

   dialog = SC_DIALOG(sc_dialog_new("System Menu", NULL, SC_DIALOG_OK | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_system_menu_apply_gtk, &setup);

   attach_option(dialog, w, "Animation",           sc_link_check_new(&setup.gfxanimate), &row);
   attach_option(dialog, w, "Graphics Are Fast",   sc_link_check_new(&setup.gfxfast), &row);
   attach_option(dialog, w, "Computers Are Fast",  sc_link_check_new(&setup.gfxcompfast), &row);

   sc_help_text(help, sizeof(help), "Mass Kill");
   button = tooltip(w, help, gtk_button_new_with_label(" Mass Kill "));
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_system_mass_kill_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_help_text(help, sizeof(help), "Erase Smoke");
   button = tooltip(w, help, gtk_button_new_with_label(" Erase Smoke "));
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_system_erase_smoke_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_help_text(help, sizeof(help), "Retreat");
   button = tooltip(w, help, gtk_button_new_with_label(" N/A:  Retreat "));
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_system_retreat_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_help_text(help, sizeof(help), "Resign Game");
   button = tooltip(w, help, gtk_button_new_with_label(" Resign Game "));
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_system_resign_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_help_text(help, sizeof(help), "Sound Setup");
   button = tooltip(w, help, gtk_button_new_with_label(" Sound Setup "));
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_system_sound_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_dialog_run(dialog);

}
