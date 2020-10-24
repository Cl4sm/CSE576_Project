static gboolean
file_exists_in_root (const gchar *root, const gchar *filename)
{
        gboolean ret = FALSE;
        GFile *source;
        gchar *source_path;

        source_path = g_build_filename (root, filename, NULL);
        source = g_file_new_for_path (source_path);

        /* ignore virtual mountpoints */
        if (!g_file_is_native (source))
                goto out;

        /* an interesting file exists */
        ret = g_file_query_exists (source, NULL);
        g_debug ("checking for %s: %s", source_path, ret ? "yes" : "no");
        if (!ret)
                goto out;
out:
        g_free (source_path);
        g_object_unref (source);
        return ret;
}