gboolean read_access(const gchar *path, GError **error)
{
    if(g_access(path, F_OK) != 0)
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 3, "%s: No such file or directory", g_strdup(path));
        }
        return FALSE;
    }
    if(g_access(path, R_OK | F_OK) != 0)
    {
        if (error)
        {
            *error = g_error_new(g_quark_from_string("bluez-tools"), 4, "%s: Read permission denied", g_strdup(path));
        }
        return FALSE;
    }
    
    return TRUE;
}
