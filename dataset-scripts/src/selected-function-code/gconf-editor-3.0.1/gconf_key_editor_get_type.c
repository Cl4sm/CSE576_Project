GType
gconf_key_editor_get_type (void)
{
	static GType object_type = 0;

	if (!object_type) {
		static const GTypeInfo object_info = {
			sizeof (GConfKeyEditorClass),
			NULL,		/* base_init */
			NULL,		/* base_finalize */
			(GClassInitFunc) gconf_key_editor_class_init,
			NULL,		/* class_finalize */
			NULL,		/* class_data */
			sizeof (GConfKeyEditor),
			0,              /* n_preallocs */
			(GInstanceInitFunc) gconf_key_editor_init
		};

		object_type = g_type_register_static (GTK_TYPE_DIALOG, "GConfKeyEditor", &object_info, 0);
	}

	return object_type;
}