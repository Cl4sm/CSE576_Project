static GList *
deja_dup_nautilus_extension_get_file_items(NautilusMenuProvider *provider,
                                           GtkWidget *window,
                                           GList *files)
{
  NautilusMenuItem *item;
  guint length;
  GList *file_copies;
  gchar *path;

  if (files == NULL)
    return NULL;

  path = g_find_program_in_path("deja-dup");
  if (!path)
    return NULL;
  g_free(path);

  gboolean is_one_included = FALSE;
  GList *p;
  for (p = files; p; p = p->next) {
    GFile *gfile = nautilus_file_info_get_location((NautilusFileInfo *)p->data);
    if (is_dir_included(gfile))
      is_one_included = TRUE;
  }
  if (!is_one_included)
    return NULL;

  length = g_list_length(files);
  item = nautilus_menu_item_new("DejaDupNautilusExtension::restore_item",
                                dngettext(GETTEXT_PACKAGE,
                                          "Revert to Previous Version…",
                                          "Revert to Previous Versions…",
                                          length),
                                dngettext(GETTEXT_PACKAGE,
                                          "Restore file from backup",
                                          "Restore files from backup",
                                          length),
                                "deja-dup");

  g_signal_connect(item, "activate", G_CALLBACK(restore_files_callback), NULL);
  g_object_set_data_full (G_OBJECT(item), "deja_dup_extension_files", 
                          nautilus_file_info_list_copy(files),
                          (GDestroyNotify)nautilus_file_info_list_free);

  return g_list_append(NULL, item);
}