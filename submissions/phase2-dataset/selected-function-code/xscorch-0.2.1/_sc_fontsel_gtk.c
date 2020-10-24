
   sc_fontsel_data_gtk setup;
   ScDialog *dialog;
   const char *font;

   setup.entry = entry;
   font = gtk_entry_get_text(GTK_ENTRY(entry));

   dialog = SC_DIALOG(sc_dialog_new(title, NULL, SC_DIALOG_OK | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_fontsel_apply_gtk, &setup);

   setup.fontsel = gtk_font_selection_new();
   sc_dialog_grid_attach(dialog, setup.fontsel, 0, 0);
   gtk_font_selection_set_font_name(GTK_FONT_SELECTION(setup.fontsel), font);

   /* GTK 2.0 has NO font filter abilities.  Numerous (user list) requests to
      revive this functionality appear to have gone completely unanswered.
      We will have to wait and hope for it to appear in newer GTK someday...
      TEMP: That was 2.2 ... now we're at 2.12 ... maybe it's back? -JTL */

   sc_dialog_run(dialog);

}
