static gboolean
ldsm_notify_for_mount (LdsmMountInfo *mount,
                       gboolean       multiple_volumes,
                       gboolean       other_usable_volumes)
{
        gchar  *name, *program;
        gint64 free_space;
        gint response;
        gboolean has_trash;
        gboolean has_disk_analyzer;
        gboolean retval = TRUE;
        gchar *path;

        /* Don't show a notice if one is already displayed */
        if (dialog != NULL || notification != NULL)
                return retval;

        name = g_unix_mount_guess_name (mount->mount);
        free_space = (gint64) mount->buf.f_frsize * (gint64) mount->buf.f_bavail;
        has_trash = ldsm_mount_has_trash (mount);
        path = g_strdup (g_unix_mount_get_mount_path (mount->mount));

        program = g_find_program_in_path (DISK_SPACE_ANALYZER);
        has_disk_analyzer = (program != NULL);
        g_free (program);

        if (server_has_actions ()) {
                char *free_space_str;
                char *summary;
                char *body;

                free_space_str = g_format_size (free_space);

                if (multiple_volumes) {
                        summary = g_strdup_printf (_("Low Disk Space on \"%s\""), name);
                        if (has_trash) {
                                body = g_strdup_printf (_("The volume \"%s\" has only %s disk space remaining.  You may free up some space by emptying the trash."),
                                                        name,
                                                        free_space_str);
                        } else {
                                body = g_strdup_printf (_("The volume \"%s\" has only %s disk space remaining."),
                                                        name,
                                                        free_space_str);
                        }
                } else {
                        summary = g_strdup (_("Low Disk Space"));
                        if (has_trash) {
                                body = g_strdup_printf (_("This computer has only %s disk space remaining.  You may free up some space by emptying the trash."),
                                                        free_space_str);
                        } else {
                                body = g_strdup_printf (_("This computer has only %s disk space remaining."),
                                                        free_space_str);
                        }
                }
                g_free (free_space_str);

                notification = notify_notification_new (summary, body, "drive-harddisk-symbolic");
                g_free (summary);
                g_free (body);

                g_signal_connect (notification,
                                  "closed",
                                  G_CALLBACK (on_notification_closed),
                                  NULL);

                notify_notification_set_app_name (notification, _("Disk space"));
                notify_notification_set_hint (notification, "transient", g_variant_new_boolean (TRUE));
                notify_notification_set_urgency (notification, NOTIFY_URGENCY_CRITICAL);
                notify_notification_set_timeout (notification, NOTIFY_EXPIRES_DEFAULT);
                if (has_disk_analyzer) {
                        notify_notification_add_action (notification,
                                                        "examine",
                                                        _("Examine"),
                                                        (NotifyActionCallback) examine_callback,
                                                        g_strdup (path),
                                                        g_free);
                }
                if (has_trash) {
                        notify_notification_add_action (notification,
                                                        "empty-trash",
                                                        _("Empty Trash"),
                                                        (NotifyActionCallback) empty_trash_callback,
                                                        NULL,
                                                        NULL);
                }
                notify_notification_add_action (notification,
                                                "ignore",
                                                _("Ignore"),
                                                (NotifyActionCallback) ignore_callback,
                                                NULL,
                                                NULL);
                notify_notification_set_category (notification, "device");

                if (!notify_notification_show (notification, NULL)) {
                        g_warning ("failed to send disk space notification\n");
                }

        } else {
                dialog = gsd_ldsm_dialog_new (other_usable_volumes,
                                              multiple_volumes,
                                              has_disk_analyzer,
                                              has_trash,
                                              free_space,
                                              name,
                                              path);

                g_object_ref (G_OBJECT (dialog));
                response = gtk_dialog_run (GTK_DIALOG (dialog));

                gtk_widget_destroy (GTK_WIDGET (dialog));
                dialog = NULL;

                switch (response) {
                case GTK_RESPONSE_CANCEL:
                        retval = FALSE;
                        break;
                case GSD_LDSM_DIALOG_RESPONSE_ANALYZE:
                        retval = FALSE;
                        ldsm_analyze_path (path);
                        break;
                case GSD_LDSM_DIALOG_RESPONSE_EMPTY_TRASH:
                        retval = TRUE;
                        gsd_ldsm_show_empty_trash ();
                        break;
                case GTK_RESPONSE_NONE:
                case GTK_RESPONSE_DELETE_EVENT:
                        retval = TRUE;
                        break;
                default:
                        g_assert_not_reached ();
                }
        }

        g_free (name);
        g_free (path);

        return retval;
}