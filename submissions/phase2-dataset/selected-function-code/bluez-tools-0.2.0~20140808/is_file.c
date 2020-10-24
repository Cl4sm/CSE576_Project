gboolean is_file(const gchar *filename, GError **error)
{
    g_assert(filename != NULL && strlen(filename) > 0);

    struct stat buf;
    if (stat(filename, &buf) != 0)
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 1, "%s: %s", g_strdup(filename), strerror(errno));
        }
        return FALSE;
    }

    if (!S_ISREG(buf.st_mode))
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 2, "%s: Invalid file", g_strdup(filename));
        }
        return FALSE;
    }

    return TRUE;
}
