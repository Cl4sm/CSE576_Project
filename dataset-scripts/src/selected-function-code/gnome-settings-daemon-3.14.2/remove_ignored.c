static void
remove_ignored (GsdUpdatesFirmware *firmware, GPtrArray *array)
{
        gboolean ret;
        gchar **ignored = NULL;
        gchar *ignored_str;
        GsdUpdatesFirmwareRequest *req;
        guint i, j;

        /* get from settings */
        ignored_str = g_settings_get_string (firmware->priv->settings,
                                             GSD_SETTINGS_IGNORED_DEVICES);
        if (ignored_str == NULL) {
                g_warning ("could not read ignored list");
                goto out;
        }

        /* nothing in list, common case */
        if (ignored_str[0] == '\0') {
                g_debug ("nothing in ignored list");
                goto out;
        }

        /* split using "," */
        ignored = g_strsplit (ignored_str, ",", 0);

        /* remove any ignored pattern matches */
        i = 0;
        while (i < array->len) {
                ret = FALSE;
                req = g_ptr_array_index (array, i);
                if (req->id == NULL)
                        continue;
                for (j=0; ignored[j] != NULL; j++) {
                        ret = g_pattern_match_simple (ignored[j], req->id);
                        if (ret) {
                                g_debug ("match %s for %s, removing", ignored[j], req->id);
                                g_ptr_array_remove_index_fast (array, i);
                                break;
                        }
                }
                if (!ret)
                        i++;
        }
out:
        g_free (ignored_str);
        g_strfreev (ignored);
}