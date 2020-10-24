static void
show_window(int img, char *label, double fraction)
{
  char *m_label;
  char *u_label;

  GtkWidget *window = mbp_w.window;

  if (img >= IMG_NIMG)
    return;

  /* Cancel timer */
  if (mbp_w.timer > 0)
    g_source_remove(mbp_w.timer);

  if (!GTK_WIDGET_VISIBLE(window))
    draw_window_bg();

  /* Put the appropriate image in there */
  if (mbp_w.image != theme.images[img])
    {
      if (mbp_w.image != NULL)
	gtk_container_remove(GTK_CONTAINER(mbp_w.img_align), mbp_w.image);

      gtk_container_add(GTK_CONTAINER(mbp_w.img_align), theme.images[img]);
    }

  mbp_w.image = theme.images[img];

  /* Set the text label */
  u_label = g_locale_to_utf8(label, -1, NULL, NULL, NULL);

  if (u_label == NULL)
    m_label = "";
  else /* accepts only UTF-8 input ... segfaults otherwise */
    m_label = g_markup_printf_escaped("<span weight=\"bold\" foreground=\"white\">%s</span>", u_label);

  gtk_label_set_markup(GTK_LABEL(mbp_w.label), m_label);

  if (u_label != NULL)
    {
      g_free(u_label);
      g_free(m_label);
    }

  /* Set the progress bar */
  if (fraction >= 0.0)
    {
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(mbp_w.pbar), fraction);

      if (!mbp_w.pbar_state)
	{
	  gtk_container_add(GTK_CONTAINER(mbp_w.pbar_align), mbp_w.pbar);
	  mbp_w.pbar_state = 1;
	}
    }
  else if (mbp_w.pbar_state)
    {
      gtk_container_remove(GTK_CONTAINER(mbp_w.pbar_align), mbp_w.pbar);
      mbp_w.pbar_state = 0;
    }

  gtk_widget_show_all(window);

  mbp_w.timer = g_timeout_add(mbp_w.timeout, hide_window, NULL);
}