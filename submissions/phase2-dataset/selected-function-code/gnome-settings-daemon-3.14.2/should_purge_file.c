static gboolean
should_purge_file (GFile        *file,
                   GCancellable *cancellable,
                   GDateTime    *old)
{
        GFileInfo *info;
        GDateTime *date;
        gboolean should_purge;

        should_purge = FALSE;

        info = g_file_query_info (file,
                                  G_FILE_ATTRIBUTE_TRASH_DELETION_DATE ","
                                  G_FILE_ATTRIBUTE_UNIX_UID ","
                                  G_FILE_ATTRIBUTE_TIME_CHANGED,
                                  G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                                  cancellable,
                                  NULL);
        if (!info)
                return FALSE;

        date = g_file_info_get_deletion_date (info);
        if (date == NULL) {
                guint uid;
                guint64 ctime;

                uid = g_file_info_get_attribute_uint32 (info, G_FILE_ATTRIBUTE_UNIX_UID);
                if (uid != getuid ()) {
                        should_purge = FALSE;
                        goto out;
                }

                ctime = g_file_info_get_attribute_uint64 (info, G_FILE_ATTRIBUTE_TIME_CHANGED);
                date = g_date_time_new_from_unix_local ((gint64) ctime);
        }

        should_purge = g_date_time_difference (old, date) >= 0;
        g_date_time_unref (date);

out:
        g_object_unref (info);

        return should_purge;
}