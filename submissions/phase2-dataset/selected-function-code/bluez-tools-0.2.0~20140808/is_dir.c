gboolean is_dir(const gchar *dirname, GError **error)
{
    g_assert(dirname != NULL && strlen(dirname) > 0);

    struct stat buf;
    if (stat(dirname, &buf) != 0)
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 1, "%s: %s", g_strdup(dirname), strerror(errno));
        }
        return FALSE;
    }

    if (!S_ISDIR(buf.st_mode))
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 2, "%s: Invalid directory", g_strdup(dirname));
        }
        return FALSE;
    }

    return TRUE;
}
