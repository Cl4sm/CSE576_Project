static PkPackage *
check_available (GsdUpdatesFirmware *firmware, const gchar *filename)
{
        guint length = 0;
        GPtrArray *array = NULL;
        GError *error = NULL;
        PkPackage *item = NULL;
        PkBitfield filter;
        PkResults *results;
        gchar **values = NULL;
        PkError *error_code = NULL;

        /* search for newest not installed package */
        filter = pk_bitfield_from_enums (PK_FILTER_ENUM_NOT_INSTALLED,
                                         PK_FILTER_ENUM_NEWEST, -1);
        values = g_strsplit (filename, "&", -1);
        results = pk_client_search_files (PK_CLIENT(firmware->priv->task),
                                          filter,
                                          values,
                                          NULL,
                                          NULL, NULL,
                                          &error);
        if (results == NULL) {
                g_warning ("failed to search file %s: %s",
                           filename, error->message);
                g_error_free (error);
                goto out;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to search file: %s, %s",
                           pk_error_enum_to_string (pk_error_get_code (error_code)),
                           pk_error_get_details (error_code));
                goto out;
        }

        /* make sure we have one package */
        array = pk_results_get_package_array (results);
        if (array->len == 0)
                g_debug ("no package providing %s found", filename);
        else if (array->len != 1)
                g_warning ("not one package providing %s found (%i)", filename, length);
        else
                item = g_object_ref (g_ptr_array_index (array, 0));
out:
        g_strfreev (values);
        if (error_code != NULL)
                g_object_unref (error_code);
        if (array != NULL)
                g_ptr_array_unref (array);
        if (results != NULL)
                g_object_unref (results);
        return item;
}