static void
install_packages_cb (GObject *object,
                     GAsyncResult *res,
                     GsdUpdatesFirmware *firmware)
{
        PkClient *client = PK_CLIENT (object);
        GError *error = NULL;
        PkResults *results = NULL;
        GPtrArray *array = NULL;
        gboolean restart = FALSE;
        const GsdUpdatesFirmwareRequest *req;
        gboolean ret;
        guint i;
        PkError *error_code = NULL;

        /* get the results */
        results = pk_client_generic_finish (client, res, &error);
        if (results == NULL) {
                g_warning ("failed to install file: %s", error->message);
                g_error_free (error);
                goto out;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to install file: %s, %s",
                           pk_error_enum_to_string (pk_error_get_code (error_code)),
                           pk_error_get_details (error_code));
                goto out;
        }

        /* go through all the requests, and find the worst type */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                ret = subsystem_can_replug (req->subsystem);
                if (!ret) {
                        restart = TRUE;
                        break;
                }
        }

        /* can we just rebind the device */
        ret = g_file_test (GSD_UPDATES_FIRMWARE_DEVICE_REBIND_PROGRAM, G_FILE_TEST_EXISTS);
        if (ret) {
                ret = device_rebind (firmware);
                if (ret) {
                        require_nothing (firmware);
                        goto out;
                }
        } else {
                /* give the user the correct message */
                if (restart)
                        require_restart (firmware);
                else
                        require_replug (firmware);
        }

        /* clear array */
        g_ptr_array_set_size (firmware->priv->array_requested, 0);
out:
        if (error_code != NULL)
                g_object_unref (error_code);
        if (array != NULL)
                g_ptr_array_unref (array);
        if (results != NULL)
                g_object_unref (results);
}