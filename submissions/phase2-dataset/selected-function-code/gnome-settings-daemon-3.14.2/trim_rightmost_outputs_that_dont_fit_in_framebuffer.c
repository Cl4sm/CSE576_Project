static gboolean
trim_rightmost_outputs_that_dont_fit_in_framebuffer (GnomeRRScreen *rr_screen, GnomeRRConfig *config)
{
        GnomeRROutputInfo **outputs;
        int i;
        gboolean applicable;
        GPtrArray *sorted_outputs;

        outputs = gnome_rr_config_get_outputs (config);
        g_return_val_if_fail (outputs != NULL, FALSE);

        /* How many are on? */

        sorted_outputs = g_ptr_array_new ();
        for (i = 0; outputs[i] != NULL; i++) {
                if (gnome_rr_output_info_is_active (outputs[i]))
                        g_ptr_array_add (sorted_outputs, outputs[i]);
        }

        /* Lay them out from left to right */

        g_ptr_array_sort (sorted_outputs, compare_output_positions);

        /* Trim! */

        applicable = FALSE;

        for (i = sorted_outputs->len - 1; i >= 0; i--) {
                GError *error = NULL;
                gboolean is_bounds_error;

                applicable = gnome_rr_config_applicable (config, rr_screen, &error);
                if (applicable)
                        break;

                is_bounds_error = g_error_matches (error, GNOME_RR_ERROR, GNOME_RR_ERROR_BOUNDS_ERROR);
                g_error_free (error);

                if (!is_bounds_error)
                        break;

                gnome_rr_output_info_set_active (sorted_outputs->pdata[i], FALSE);
        }

        if (config_is_all_off (config))
                applicable = FALSE;

        g_ptr_array_free (sorted_outputs, FALSE);

        return applicable;
}