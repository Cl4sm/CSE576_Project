static gboolean
gsd_xrandr_manager_2_rotate_to (GsdXrandrManager *manager,
                                GnomeRRRotation   rotation,
                                gint64            timestamp,
                                GError          **error)
{
        guint i;
        gboolean found;

        found = FALSE;
        for (i = 0; i < G_N_ELEMENTS (possible_rotations); i++) {
                if (rotation == possible_rotations[i]) {
                        found = TRUE;
                        break;
                }
        }

        if (found == FALSE) {
                g_debug ("Not setting out of bounds rotation '%d'", rotation);
                return FALSE;
        }

        handle_rotate_windows (manager, rotation, timestamp);
        return TRUE;
}