static gboolean
gconf_editor_window_test_expand_row (GtkTreeView       *tree_view,
				     GtkTreeIter       *iter,
				     GtkTreePath       *path,
				     gpointer           data)
{
  GConfEditorWindow *gconfwindow = GCONF_EDITOR_WINDOW (data);
  GdkCursor *cursor;

  if (!gtk_widget_get_realized (GTK_WIDGET (gconfwindow)))
    return FALSE;

  cursor = gdk_cursor_new (GDK_WATCH);
  gdk_window_set_cursor (gtk_widget_get_window (GTK_WIDGET (gconfwindow)),
                         cursor);
  gdk_cursor_unref (cursor);

  gdk_display_flush (gtk_widget_get_display (GTK_WIDGET (gconfwindow)));

  return FALSE;
}