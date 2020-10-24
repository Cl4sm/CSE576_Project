static void create_plugin_tab(GtkWidget *tab_vbox)
{
  GtkWidget *tabs = NULL;
  GtkWidget *options_tab = NULL;
  GtkWidget *info_tab = NULL;
  GtkWidget *info = NULL;
  GtkWidget *about_tab = NULL;
  GtkWidget *about = NULL;
  GtkWidget *main_box;
  
  static gchar *info_text[] =
    {
      "<b>" PLUGIN_NAME "\n\n",
      PLUGIN_DESC "\n\n",
      "improve your productivity with " PLUGIN_NAME ":\n",
      "It monitors your hard-drives by sending visual stimuli to your cortex\n",
      "each time your /dev/hdx writes or reads anything.\n",
      "Try to launch openoffice and enjoy the gkhdplop show!\n\n",
      "<i>Usage:\n\n",
      "- Change the hard drive with the mouse wheel\n",
      "- Animation reflecting swap activity...\n",
      "- Background animation reflecting disk activity...\n",
      "- Small led indicating disk activity...\n",
      "- Several glittering color schemes...\n",
      "- Popup display with the io throughput...\n",
    };
  
  static gchar *about_text =
    _(
      PLUGIN_NAME " " PLUGIN_VERSION "\n"
      "a " PLUGIN_DESC "\n\n"
      "Copyright (c) 2004 dae\n"
      "jolly.frederic@fnac.net\n"
      "http://quadaemon.free.fr\n\n"
      "Based on wmhdplop\nCopyright (c) 2003,2004 pouaite\n"
      PLUGIN_URL "\n\n"
      "Released under the GNU Public Licence"
      );

  tabs = gtk_notebook_new();
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(tabs), GTK_POS_TOP);
  gtk_box_pack_start(GTK_BOX(tab_vbox), tabs, TRUE, TRUE, 0);
  
  /* Options tab */
  options_tab = gkrellm_gtk_notebook_page(tabs, _("Options"));
  
  main_box = gtk_vbox_new(FALSE, 0);
  gtk_widget_set_name(main_box, "main_box");
  gtk_widget_ref(main_box);
  gtk_object_set_data_full(GTK_OBJECT (options_tab), "main_box", main_box,
			   (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show(main_box);
  gtk_container_add(GTK_CONTAINER (options_tab), main_box);

  /* io button */
  gkrellm_gtk_check_button_connected(main_box, &io_check, !Prefs.disable_io_matrix, 1, 1, 0,
				     option_toggled_cb, NULL, _("Show disk activity"));
  /* swap button */
  gkrellm_gtk_check_button_connected(main_box, &swap_check, !Prefs.disable_swap_matrix, 1, 1, 0,
				     option_toggled_cb, NULL, _("Show swap activity"));
  /* hdlist button */
  gkrellm_gtk_check_button_connected(main_box, &hdlist_check, 
				     (Prefs.hdlist_pos == AL_NONE)?FALSE:TRUE, 1, 1, 0,
				     option_toggled_cb, NULL, _("Show disk name"));
  /* leds button */
  gkrellm_gtk_check_button_connected(main_box, &leds_check, !Prefs.disable_hd_leds, 1, 1, 0,
				     option_toggled_cb, NULL, _("Led indicating disk activity"));
  gtk_widget_set_sensitive(leds_check, (Prefs.hdlist_pos == AL_NONE)?FALSE:TRUE);
  /* hddtemp button */
  gkrellm_gtk_check_button_connected(main_box, &hddtemp_check, Prefs.enable_hddtemp, 1, 1, 0,
				     option_toggled_cb, NULL,
				     _("Display hd temperature (requires hddtemp daemon running on port 7634)"));
  gtk_widget_set_sensitive(hddtemp_check, (Prefs.hdlist_pos == AL_NONE)?FALSE:TRUE);
  /* colormap spin button */
  gkrellm_gtk_spin_button(main_box, NULL, Prefs.iomatrix_colormap, 0, 4, 1, 1, 0, 0,
			  cb_colormap_modified, NULL, FALSE, _("Colormap"));
  
  /* threshold spin button */
  gkrellm_gtk_spin_button(main_box, &throughput_threshold, Prefs.popup_throughput_threshold, 0., 500, 0.1, 1, 1, 0,
                          cb_spinbutton_modified, NULL, FALSE, _("minimum io throughput (MB/s)"));
  
  {
    GtkWidget *frame = gtk_frame_new("Fonts");
    gtk_box_pack_start(GTK_BOX(main_box), frame, TRUE, FALSE, 0);

    GtkWidget *hbox0 = gtk_hbox_new(FALSE, 4);
    gtk_container_add(GTK_CONTAINER(frame), hbox0); 

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox0), vbox, TRUE, FALSE, 0);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 0);
    entry_smallfont = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_smallfont, TRUE, FALSE, 0);
    GtkWidget *pLabel = gtk_label_new("Small font (without .ttf extension):");
    gtk_entry_set_text(GTK_ENTRY(entry_smallfont), app->current_smallfont_name);
    gtk_box_pack_start(GTK_BOX(hbox), pLabel, TRUE, FALSE, 0);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 0);
    entry_bigfont = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_bigfont, TRUE, FALSE, 0);
    gtk_entry_set_text(GTK_ENTRY(entry_bigfont), app->current_bigfont_name);
    pLabel = gtk_label_new("Big font (without .ttf extension)");
    gtk_box_pack_start(GTK_BOX(hbox), pLabel, TRUE, FALSE, 0);

    gkrellm_gtk_button_connected(hbox0, NULL, TRUE/* expand*/, TRUE /* fill*/, 0 /*pad*/,
                                 G_CALLBACK(cb_reload_fonts), NULL, "reload fonts");
  }

  /* Info tab */
  info_tab = gkrellm_gtk_framed_notebook_page(tabs, _("Info"));
  info = gkrellm_gtk_scrolled_text_view(info_tab, NULL, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gkrellm_gtk_text_view_append_strings(info, info_text, sizeof(info_text) / sizeof(gchar *));

  /* About tab */
  about_tab = gkrellm_gtk_notebook_page(tabs, _("About"));
  about = gtk_label_new(about_text);
  gtk_box_pack_start(GTK_BOX(about_tab), about, TRUE, TRUE, 0);
}