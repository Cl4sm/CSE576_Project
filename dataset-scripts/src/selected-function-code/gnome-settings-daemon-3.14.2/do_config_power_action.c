static void
do_config_power_action (GsdMediaKeysManager *manager,
                        const gchar *config_key,
                        gboolean in_lock_screen)
{
        GsdPowerActionType action_type;

        action_type = g_settings_get_enum (manager->priv->power_settings,
                                           config_key);
        switch (action_type) {
        case GSD_POWER_ACTION_SUSPEND:
                power_action (manager, "Suspend", !in_lock_screen);
                break;
        case GSD_POWER_ACTION_INTERACTIVE:
                if (!in_lock_screen)
                        gnome_session_shutdown (manager);
                break;
        case GSD_POWER_ACTION_SHUTDOWN:
                power_action (manager, "PowerOff", !in_lock_screen);
                break;
        case GSD_POWER_ACTION_HIBERNATE:
                power_action (manager, "Hibernate", !in_lock_screen);
                break;
        case GSD_POWER_ACTION_BLANK:
        case GSD_POWER_ACTION_LOGOUT:
        case GSD_POWER_ACTION_NOTHING:
                /* these actions cannot be handled by media-keys and
                 * are not used in this context */
                break;
        }
}