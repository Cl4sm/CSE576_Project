static GnomeRRConfig *
make_laptop_setup (GsdXrandrManager *manager, GnomeRRScreen *screen)
{
        /* Turn on the laptop, disable everything else */
        GnomeRRConfig *result = gnome_rr_config_new_current (screen, NULL);
        GnomeRROutputInfo **outputs = gnome_rr_config_get_outputs (result);
        int i;

        gnome_rr_config_set_clone (result, FALSE);

        for (i = 0; outputs[i] != NULL; ++i) {
                GnomeRROutputInfo *info = outputs[i];

                if (is_laptop (screen, info) && !laptop_lid_is_closed (manager)) {
                        if (!turn_on (screen, info, 0, 0)) {
                                g_object_unref (G_OBJECT (result));
                                result = NULL;
                                break;
                        }
                }
                else {
                        gnome_rr_output_info_set_active (info, FALSE);
                }
        }


        if (config_is_all_off (result)) {
                g_object_unref (G_OBJECT (result));
                result = NULL;
        }

        print_configuration (result, "Laptop setup");

        /* FIXME - Maybe we should return NULL if there is more than
         * one connected "laptop" screen?
         */
        return result;

}