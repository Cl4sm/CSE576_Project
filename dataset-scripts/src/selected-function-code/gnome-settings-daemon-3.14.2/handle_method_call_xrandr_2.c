static void
handle_method_call_xrandr_2 (GsdXrandrManager *manager,
                             const gchar *method_name,
                             GVariant *parameters,
                             GDBusMethodInvocation *invocation)
{
        gint64 timestamp;

        g_debug ("Calling method '%s' for org.gnome.SettingsDaemon.XRANDR_2", method_name);

        if (g_strcmp0 (method_name, "VideoModeSwitch") == 0) {
                g_variant_get (parameters, "(x)", &timestamp);
                gsd_xrandr_manager_2_video_mode_switch (manager, timestamp, NULL);
                g_dbus_method_invocation_return_value (invocation, NULL);
        } else if (g_strcmp0 (method_name, "Rotate") == 0) {
                g_variant_get (parameters, "(x)", &timestamp);
                gsd_xrandr_manager_2_rotate (manager, timestamp, NULL);
                g_dbus_method_invocation_return_value (invocation, NULL);
        } else if (g_strcmp0 (method_name, "RotateTo") == 0) {
                GnomeRRRotation rotation;
                g_variant_get (parameters, "(ix)", &rotation, &timestamp);
                gsd_xrandr_manager_2_rotate_to (manager, rotation, timestamp, NULL);
                g_dbus_method_invocation_return_value (invocation, NULL);
        }
}