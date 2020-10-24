static void
convert_libgnomekbd_layouts (GSettings *settings)
{
        GVariantBuilder builder;
        GSettings *libgnomekbd_settings;
        gchar **layouts, **l;

        if (!schema_is_installed ("org.gnome.libgnomekbd.keyboard"))
                return;

        init_builder_with_sources (&builder, settings);

        libgnomekbd_settings = g_settings_new ("org.gnome.libgnomekbd.keyboard");
        layouts = g_settings_get_strv (libgnomekbd_settings, "layouts");

        for (l = layouts; *l; ++l) {
                gchar *id;
                gchar **strv;

                strv = g_strsplit (*l, "\t", 2);
                if (strv[0] && !strv[1])
                        id = g_strdup (strv[0]);
                else if (strv[0] && strv[1])
                        id = g_strdup_printf ("%s+%s", strv[0], strv[1]);
                else
                        id = NULL;

                if (id)
                        g_variant_builder_add (&builder, "(ss)", INPUT_SOURCE_TYPE_XKB, id);

                g_free (id);
                g_strfreev (strv);
        }

        g_settings_set_value (settings, KEY_INPUT_SOURCES, g_variant_builder_end (&builder));

        g_strfreev (layouts);
        g_object_unref (libgnomekbd_settings);
}