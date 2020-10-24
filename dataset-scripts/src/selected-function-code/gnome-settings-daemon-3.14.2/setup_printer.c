static gboolean
setup_printer (gchar *device_id,
               gchar *device_make_and_model,
               gchar *device_uri)
{
        gboolean  success = FALSE;
        gchar    *ppd_name;
        gchar    *printer_name;

        ppd_name = get_best_ppd (device_id, device_make_and_model, device_uri);
        printer_name = create_name (device_id);

        if (!ppd_name || !printer_name || !device_uri) {
                g_free (ppd_name);
                g_free (printer_name);
                return FALSE;
        }

        success = add_printer (printer_name, device_uri,
                               ppd_name, NULL, NULL);

        /* Set some options of the new printer */
        if (success) {
                const char *ppd_file_name;

                printer_set_accepting_jobs (printer_name, TRUE, NULL);
                printer_set_enabled (printer_name, TRUE);
                printer_autoconfigure (printer_name);

                ppd_file_name = cupsGetPPD (printer_name);

                if (ppd_file_name) {
                        GHashTable *executables;
                        GHashTable *packages;

                        set_default_paper_size (printer_name, ppd_file_name);

                        executables = get_missing_executables (ppd_file_name);
                        packages = find_packages_for_executables (executables);
                        install_packages (packages);

                        if (executables)
                                g_hash_table_destroy (executables);
                        if (packages)
                                g_hash_table_destroy (packages);
                        g_unlink (ppd_file_name);
                }
        }

        g_free (printer_name);
        g_free (ppd_name);

        return success;
}