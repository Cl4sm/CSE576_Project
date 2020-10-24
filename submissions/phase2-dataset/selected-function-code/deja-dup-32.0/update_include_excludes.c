static void
update_include_excludes ()
{
  /* Clear any existing dirs */
  if (dirs != NULL) {
    g_list_foreach(dirs, (GFunc)g_object_unref, NULL);
    g_list_free(dirs);
    dirs = NULL;
  }

  if (settings == NULL)
    return;

  gchar **includes_strv = g_settings_get_strv(settings, "include-list");
  gchar **excludes_strv = g_settings_get_strv(settings, "exclude-list");

  gchar **p;
  for (p = includes_strv; p && *p; p++) {
    GFile *file = deja_dup_parse_dir(*p);
    if (file != NULL) {
      g_object_set_data(G_OBJECT(file), "included", GINT_TO_POINTER(TRUE));
      dirs = g_list_insert_sorted(dirs, file, (GCompareFunc)cmp_prefix);
    }
  }
  for (p = excludes_strv; p && *p; p++) {
    GFile *file = deja_dup_parse_dir(*p);
    if (file != NULL) {
      g_object_set_data(G_OBJECT(file), "included", GINT_TO_POINTER(FALSE));
      dirs = g_list_insert_sorted(dirs, file, (GCompareFunc)cmp_prefix);
    }
  }


  g_strfreev(includes_strv);
  g_strfreev(excludes_strv);
}