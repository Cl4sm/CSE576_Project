static void
package_download_finished_cb (GObject *object,
                              GAsyncResult *res,
                              GsdUpdatesManager *manager)
{
        PkClient *client = PK_CLIENT(object);
        PkResults *results;
        GError *error = NULL;
        PkError *error_code = NULL;

        /* get the results */
        results = pk_client_generic_finish (PK_CLIENT(client), res, &error);
        if (results == NULL) {
                if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
                        g_error_free (error);
                        return;
                }
                g_warning ("failed to download: %s",
                           error->message);
                g_error_free (error);
                notify_failed_get_updates_maybe (manager);
                return;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to download: %s, %s",
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

        /* check to see if should notify */
        check_updates_for_importance (manager);

out:
        if (error_code != NULL)
                g_object_unref (error_code);
        if (results != NULL)
                g_object_unref (results);
}