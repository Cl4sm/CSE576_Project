static void
create_window(void)
{
  GdkScreen *screen;
  GdkColormap *rgba;

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *align;

  window = gtk_window_new(GTK_WINDOW_POPUP);
  screen = gtk_widget_get_screen (window);

  if (gdk_screen_is_composited (screen))
    {
      rgba = gdk_screen_get_rgba_colormap (screen);
      gtk_widget_set_colormap (window, rgba);
    }

  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
  gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);

  gtk_window_set_default_size(GTK_WINDOW(window), theme.width, theme.height);
  gtk_widget_set_size_request(GTK_WIDGET(window), theme.width, theme.height);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /* Image */
  mbp_w.img_align = gtk_alignment_new(0.5, 0.7, 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), mbp_w.img_align, TRUE, TRUE, 0);

  /* Text message */
  align = gtk_alignment_new(0.5, 0.0, 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), align, TRUE, TRUE, 0);

  mbp_w.label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER(align), mbp_w.label);

  /* Progress bar */
  mbp_w.pbar_align = gtk_alignment_new(0.5, 0.0, 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(vbox), mbp_w.pbar_align, TRUE, TRUE, 0);

  mbp_w.pbar = gtk_progress_bar_new();
  /* make it 10px high */
  gtk_widget_set_size_request(mbp_w.pbar, -1, 10);
  gtk_container_add(GTK_CONTAINER(mbp_w.pbar_align), mbp_w.pbar);
  /* Up the refcount to prevent GTK from freeing the widget */
  gtk_widget_ref(mbp_w.pbar);

  mbp_w.pbar_state = 1;
  mbp_w.window = window;
  mbp_w.image = NULL;
  mbp_w.timer = 0;

  gtk_widget_realize(GTK_WIDGET(window));

  if (gdk_screen_is_composited(screen))
    {
      gdk_window_set_opacity(window->window, 0.8);
      g_signal_connect_after(window, "expose-event", G_CALLBACK(window_expose_event), NULL);
    }
}