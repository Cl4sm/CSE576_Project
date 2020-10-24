static void
ldsm_maybe_warn_mounts (GList *mounts,
                        gboolean multiple_volumes,
                        gboolean other_usable_volumes)
{
        GList *l;
        gboolean done = FALSE;

        for (l = mounts; l != NULL; l = l->next) {
                LdsmMountInfo *mount_info = l->data;
                LdsmMountInfo *previous_mount_info;
                gdouble free_space;
                gdouble previous_free_space;
                time_t curr_time;
                const gchar *path;
                gboolean show_notify;

                if (done) {
                        /* Don't show any more dialogs if the user took action with the last one. The user action
                         * might free up space on multiple volumes, making the next dialog redundant.
                         */
                        ldsm_free_mount_info (mount_info);
                        continue;
                }

                path = g_unix_mount_get_mount_path (mount_info->mount);

                previous_mount_info = g_hash_table_lookup (ldsm_notified_hash, path);
                if (previous_mount_info != NULL)
                        previous_free_space = (gdouble) previous_mount_info->buf.f_bavail / (gdouble) previous_mount_info->buf.f_blocks;

                free_space = (gdouble) mount_info->buf.f_bavail / (gdouble) mount_info->buf.f_blocks;

                if (previous_mount_info == NULL) {
                        /* We haven't notified for this mount yet */
                        show_notify = TRUE;
                        mount_info->notify_time = time (NULL);
                        g_hash_table_replace (ldsm_notified_hash, g_strdup (path), mount_info);
                } else if ((previous_free_space - free_space) > free_percent_notify_again) {
                        /* We've notified for this mount before and free space has decreased sufficiently since last time to notify again */
                        curr_time = time (NULL);
                        if (difftime (curr_time, previous_mount_info->notify_time) > (gdouble)(min_notify_period * 60)) {
                                show_notify = TRUE;
                                mount_info->notify_time = curr_time;
                        } else {
                                /* It's too soon to show the dialog again. However, we still replace the LdsmMountInfo
                                 * struct in the hash table, but give it the notfiy time from the previous dialog.
                                 * This will stop the notification from reappearing unnecessarily as soon as the timeout expires.
                                 */
                                show_notify = FALSE;
                                mount_info->notify_time = previous_mount_info->notify_time;
                        }
                        g_hash_table_replace (ldsm_notified_hash, g_strdup (path), mount_info);
                } else {
                        /* We've notified for this mount before, but the free space hasn't decreased sufficiently to notify again */
                        ldsm_free_mount_info (mount_info);
                        show_notify = FALSE;
                }

                if (show_notify) {
                        if (ldsm_notify_for_mount (mount_info, multiple_volumes, other_usable_volumes))
                                done = TRUE;
                }
        }
}