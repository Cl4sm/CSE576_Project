static void
get_updates_finished_cb (GObject *object,
                         GAsyncResult *res,
                         GsdUpdatesManager *manager)
{
        PkClient *client = PK_CLIENT(object);
        PkResults *results;
        GError *error = NULL;
        gboolean ret;
        PkError *error_code = NULL;

        /* get the results */
        results = pk_client_generic_finish (PK_CLIENT(client), res, &error);
        if (results == NULL) {
                if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
                        g_error_free (error);
                        return;
                }
                g_warning ("failed to get updates: %s",
                           error->message);
                g_error_free (error);
                notify_failed_get_updates_maybe (manager);
                goto out;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to get updates: %s, %s",
                           pk_error_enum_to_string (pk_error_get_code (error_code)),
                           pk_error_get_details (error_code));
                switch (pk_error_get_code (error_code)) {
                case PK_ERROR_ENUM_CANCELLED_PRIORITY:
                case PK_ERROR_ENUM_TRANSACTION_CANCELLED:
                        g_debug ("ignoring error");
                        break;
                default:
                        notify_failed_get_updates_maybe (manager);
                        break;
                }
                goto out;
        }

        /* we succeeded, so clear the count */
        manager->priv->failed_get_updates_count = 0;

        /* so we can download or check for important & security updates */
        if (manager->priv->update_packages != NULL)
                g_ptr_array_unref (manager->priv->update_packages);
        manager->priv->update_packages = pk_results_get_package_array (results);

        /* we have no updates */
        if (manager->priv->update_packages->len == 0) {
                g_debug ("no updates");
                goto out;
        }

        /* should we auto-download the updates? */
        ret = g_settings_get_boolean (manager->priv->settings_gsd,
                                      GSD_SETTINGS_AUTO_DOWNLOAD_UPDATES);
        if (ret) {
                auto_download_updates (manager);
                goto out;
        }

        /* just check to see if should notify */
        check_updates_for_importance (manager);
out:
        if (error_code != NULL)
                g_object_unref (error_code);
        if (results != NULL)
                g_object_unref (results);
}