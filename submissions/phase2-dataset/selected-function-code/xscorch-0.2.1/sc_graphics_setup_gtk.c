void sc_graphics_setup_gtk(sc_window_gtk *w) {

   sc_config *c = w->c;
   sc_graphics_setup_data_gtk setup;
   ScDialog *dialog;
   GtkWidget *button;
   int confirm = (SC_NETWORK_AUTH(c) ? SC_DIALOG_OK | SC_DIALOG_APPLY : 0);
   int row = 0;

   setup.c = c;
   setup.fieldwidth  = c->fieldwidth;
   setup.fieldheight = c->fieldheight;
   setup.gfxdither   = c->graphics.gfxdither;
   setup.gfxanimate  = c->graphics.gfxanimate;
   setup.gfxfast     = c->graphics.gfxfast;
   setup.gfxcompfast = c->graphics.gfxcompfast;

   dialog = SC_DIALOG(sc_dialog_new("Graphics Setup", NULL, confirm | SC_DIALOG_CANCEL));
   g_signal_connect(G_OBJECT(dialog), "apply",
                    (GCallback)_sc_graphics_setup_apply_gtk, &setup);

   attach_option(dialog, w, "Screen Width",        sc_link_spin_new(&setup.fieldwidth, SC_MIN_FIELD_WIDTH, SC_MAX_FIELD_WIDTH, 8), &row);
   attach_option(dialog, w, "Screen Height",       sc_link_spin_new(&setup.fieldheight, SC_MIN_FIELD_HEIGHT, SC_MAX_FIELD_HEIGHT, 8), &row);
   attach_option(dialog, w, "Dithering",           sc_link_check_new(&setup.gfxdither), &row);
   attach_option(dialog, w, "Animation",           sc_link_check_new(&setup.gfxanimate), &row);
   attach_option(dialog, w, "Graphics Are Fast",   sc_link_check_new(&setup.gfxfast), &row);
   attach_option(dialog, w, "Computers Are Fast",  sc_link_check_new(&setup.gfxcompfast), &row);
   
   button = gtk_button_new_with_label(" Setup Fonts ");
   g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(sc_font_gtk), w);  /*swapped args for sc_font_gtk */
   sc_dialog_grid_attach(dialog, button, row++, 1);

   sc_dialog_run(dialog);

}
