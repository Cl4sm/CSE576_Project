static void
draw_window_bg(void)
{
  GtkWidget *window = mbp_w.window;

  GdkWindow *root_win;
  GdkScreen *screen;
  GdkRectangle mon_size;
  GdkPixbuf *pixbuf = NULL;
  GdkPixmap *pixmap = NULL;

  int x, y;
  int monitor;

  screen = gtk_window_get_screen(GTK_WINDOW(window));

  /* Find which monitor the mouse cursor is on */
  root_win = gdk_screen_get_root_window(screen);
  gdk_window_get_pointer(root_win, &x, &y, NULL);

  monitor = gdk_screen_get_monitor_at_point(screen, x, y);
  gdk_screen_get_monitor_geometry(screen, monitor, &mon_size);

  /* Move the window to the bottom center of the screen */
  x = mon_size.x + (mon_size.width - theme.width) / 2;
  y = mon_size.y + (mon_size.height - 100 - theme.height);

  gtk_window_move(GTK_WINDOW(window), x, y);

  /* Redraw the window background, compositing the background pixmap with
   * the portion of the root window that's beneath the window
   */
  pixbuf = gdk_pixbuf_get_from_drawable(NULL,
					gdk_get_default_root_window(), gdk_colormap_get_system(),
					x, y, 0, 0, theme.width, theme.height);

  /* render the combined pixbuf to a pixmap with alpha control */
  pixmap = gdk_pixmap_new(GTK_WIDGET(window)->window, theme.width, theme.height, -1);
  GdkGC *gc = gdk_gc_new(pixmap);
  gdk_draw_rectangle(pixmap, gc, TRUE, 0, 0, theme.width, theme.height);
  if (!gdk_screen_is_composited(screen))
  {
    gdk_draw_pixbuf(pixmap, NULL, pixbuf, 0, 0, 0, 0,
		    theme.width, theme.height, GDK_RGB_DITHER_NONE, 0, 0);
  }

  gdk_draw_pixbuf(pixmap, NULL, theme.background, 0, 0, 0, 0,
		  theme.width, theme.height, GDK_RGB_DITHER_NONE, 0, 0);
  gdk_window_set_back_pixmap(GTK_WIDGET(window)->window, pixmap, FALSE);

  g_object_unref(pixbuf);
  g_object_unref(pixmap);
}