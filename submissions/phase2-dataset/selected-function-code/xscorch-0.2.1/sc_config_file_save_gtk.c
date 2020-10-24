
   sc_file_save save;
   ScDialog  *dialog;
   int row = 0;
   
   save.c = w->c;
   strcopyb(save.name, w->c->cfreg->filename, sizeof(save.name));
   
   dialog = SC_DIALOG(sc_dialog_new("Save Options As ...", NULL, SC_DIALOG_OK | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_config_file_save_apply_gtk, &save);

   attach_option_help(dialog, w, "Save options in file", 
                      "Specify the name of a file to save your user configuration to.", 
                      sc_link_entry_new(save.name, SC_FILENAME_LENGTH), &row);

   sc_dialog_run(dialog);

}
