static void
remove_banned (GsdUpdatesFirmware *firmware, GPtrArray *array)
{
        gboolean ret;
        gchar **banned = NULL;
        gchar *banned_str;
        GsdUpdatesFirmwareRequest *req;
        guint i, j;

        /* get from settings */
        banned_str = g_settings_get_string (firmware->priv->settings,
                                            GSD_SETTINGS_BANNED_FIRMWARE);
        if (banned_str == NULL) {
                g_warning ("could not read banned list");
                goto out;
        }

        /* nothing in list, common case */
        if (banned_str[0] == '\0') {
                g_debug ("nothing in banned list");
                goto out;
        }

        /* split using "," */
        banned = g_strsplit (banned_str, ",", 0);

        /* remove any banned pattern matches */
        i = 0;
        while (i < array->len) {
                ret = FALSE;
                req = g_ptr_array_index (array, i);
                for (j=0; banned[j] != NULL; j++) {
                        ret = g_pattern_match_simple (banned[j], req->filename);
                        if (ret) {
                                g_debug ("match %s for %s, removing",
                                         banned[j], req->filename);
                                g_ptr_array_remove_index_fast (array, i);
                                break;
                        }
                }
                if (!ret)
                        i++;
        }
out:
        g_free (banned_str);
        g_strfreev (banned);
}