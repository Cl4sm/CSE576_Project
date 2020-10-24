void
gsd_smartcard_utils_register_error_domain (GQuark error_domain,
                                           GType  error_enum)
{
        const char *error_domain_string;
        char *type_name;
        GType type;
        GTypeClass *type_class;
        GEnumClass *enum_class;
        guint i;

        error_domain_string = g_quark_to_string (error_domain);
        type_name = dashed_string_to_studly_caps (error_domain_string);
        type = g_type_from_name (type_name);
        type_class = g_type_class_ref (type);
        enum_class = G_ENUM_CLASS (type_class);

        for (i = 0; i < enum_class->n_values; i++) {
                char *dbus_error_string;

                dbus_error_string = dashed_string_to_dbus_error_string (error_domain_string,
                                                                        "gsd",
                                                                        "org.gnome.SettingsDaemon",
                                                                        enum_class->values[i].value_nick);

                g_debug ("%s: Registering dbus error %s", type_name, dbus_error_string);
                g_dbus_error_register_error (error_domain,
                                             enum_class->values[i].value,
                                             dbus_error_string);
                g_free (dbus_error_string);
        }

        g_type_class_unref (type_class);
}