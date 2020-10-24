void 
config_gui(void)
{
  GtkWidget *content;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *img;

  struct dirent **namelist;
  const char *cur_theme;
  int n; 

  app_window = gtk_dialog_new_with_buttons(_("gpomme preferences"), NULL, 0,
					   GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT,
					   NULL);

  gtk_window_set_default_size(GTK_WINDOW(app_window), 400, 240);
  gtk_window_set_resizable(GTK_WINDOW(app_window), TRUE);

  content = gtk_dialog_get_content_area(GTK_DIALOG(app_window));
  gtk_box_set_spacing(GTK_BOX(content), 10);

  /* Theme */
  vbox = gtk_vbox_new(FALSE, 10);

  label = gtk_label_new(_("Theme:"));
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 1.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

  hbox = gtk_hbox_new(FALSE, 10);

  img = gtk_image_new_from_icon_name("gnome-settings-theme", GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, TRUE, 0);

  cb_theme = gtk_combo_box_new_text();
  gtk_box_pack_start(GTK_BOX(hbox), cb_theme, TRUE, TRUE, 10);

  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(content), vbox, FALSE, TRUE, 10);

  /* Timeout */
  vbox = gtk_vbox_new(FALSE, 10);

  label = gtk_label_new(_("Timeout (seconds):"));
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 1.0);
  gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

  hbox = gtk_hbox_new(FALSE, 10);

  img = gtk_image_new_from_icon_name("appointment", GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, TRUE, 0);

  hs_timeout = gtk_hscale_new_with_range(0.0, 5.0, 0.1);
  gtk_range_set_value(GTK_RANGE(hs_timeout), (gdouble)cfg_getint(cfg, "timeout") / 1000.0);
  gtk_box_pack_start(GTK_BOX(hbox), hs_timeout, TRUE, TRUE, 10);

  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(content), vbox, FALSE, TRUE, 10);

  /* Populate combo box */
  cur_theme = cfg_getstr(cfg, "theme");

  gtk_combo_box_append_text(GTK_COMBO_BOX(cb_theme), cur_theme);
  gtk_combo_box_set_active(GTK_COMBO_BOX(cb_theme), 0);

  n = scandir(THEME_BASE, &namelist, 0, alphasort); 
  if (n < 0)
    {
      fprintf(stderr, "Could not open theme directory: %s\n", strerror(errno));

      exit(1);
    }

  while (n--)
    {
      if ((namelist[n]->d_name[0] != '.')
	  && (strcmp(namelist[n]->d_name, cur_theme) != 0))
	{
	  /* printf("%s\n", namelist[n]->d_name); */
	  gtk_combo_box_append_text(GTK_COMBO_BOX(cb_theme), namelist[n]->d_name); 
	}
    }

  /* signals... */
  g_signal_connect(app_window, "response", G_CALLBACK(on_gpomme_response_cb), NULL);
  g_signal_connect(app_window, "close", G_CALLBACK(on_gpomme_window_close_cb), NULL);

  g_signal_connect(hs_timeout, "value-changed", G_CALLBACK(update_gui_config), NULL);
  g_signal_connect(cb_theme, "changed", G_CALLBACK(update_gui_config), NULL);

  gtk_widget_show_all(app_window);

  gtk_main();
}