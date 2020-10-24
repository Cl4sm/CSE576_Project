static void
refresh_cache_finished_cb (GObject *object, GAsyncResult *res, GsdUpdatesManager *manager)
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
                g_warning ("failed to refresh the cache: %s",
                           error->message);
                g_error_free (error);
                return;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to refresh the cache: %s, %s",
                           pk_error_enum_to_string (pk_error_get_code (error_code)),
                           pk_error_get_details (error_code));
        }

        if (error_code != NULL)
                g_object_unref (error_code);
        if (results != NULL)
                g_object_unref (results);
}