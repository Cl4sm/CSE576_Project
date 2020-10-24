void sc_font_gtk(sc_window_gtk *w) {

   GtkWidget *button;
   int row = 0;

   sc_config *c = w->c;
   sc_font_data_gtk setup;
   ScDialog *dialog;

   setup.w = w;
   strcopyb(setup.fixed_font,          c->fixed_font,          SC_FONT_LENGTH);
   strcopyb(setup.italic_fixed_font,   c->italic_fixed_font,   SC_FONT_LENGTH);
   strcopyb(setup.bold_fixed_font,     c->bold_fixed_font,     SC_FONT_LENGTH);

   dialog = SC_DIALOG(sc_dialog_new("Font Selection", NULL, SC_DIALOG_APPLY | SC_DIALOG_OK | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_font_apply_gtk, &setup);

   sc_dialog_grid_attach(dialog, sc_label_new("Fixed font"), row, 0);
   setup.fixed_entry = sc_link_entry_new(setup.fixed_font, SC_FONT_LENGTH);
   sc_dialog_grid_attach(dialog, setup.fixed_entry, row, 1);
   button = gtk_button_new_with_label(" Change ");
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_fontsel_fixed_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row, 2);
   ++row;
                  
   sc_dialog_grid_attach(dialog, sc_label_new("Italic fixed font"), row, 0);
   setup.italic_fixed_entry = sc_link_entry_new(setup.italic_fixed_font, SC_FONT_LENGTH);
   sc_dialog_grid_attach(dialog, setup.italic_fixed_entry, row, 1);
   button = gtk_button_new_with_label(" Change ");
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_fontsel_italic_fixed_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row, 2);
   ++row;
                  
   sc_dialog_grid_attach(dialog, sc_label_new("Bold fixed font"), row, 0);
   setup.bold_fixed_entry = sc_link_entry_new(setup.bold_fixed_font, SC_FONT_LENGTH);
   sc_dialog_grid_attach(dialog, setup.bold_fixed_entry, row, 1);
   button = gtk_button_new_with_label(" Change ");
   g_signal_connect(G_OBJECT(button), "clicked",
                    (GCallback)_sc_fontsel_bold_fixed_gtk, &setup);
   sc_dialog_grid_attach(dialog, button, row, 2);
   ++row;
                  
   sc_dialog_run(dialog);

}
