static void
gconf_editor_window_set_property (GObject      *object,
				  guint         param_id,
				  const GValue *value,
				  GParamSpec   *pspec)
{
	GConfEditorWindow *gconfwindow = (GConfEditorWindow *) object;

	switch (param_id) {
	case PROP_TYPE:
		/* Construct-only */
		gconfwindow->type = g_value_get_int (value);
		gconfwindow->client = gconf_editor_window_get_client (gconfwindow->type);
		switch (gconfwindow->type) {
			case GCONF_EDITOR_WINDOW_TYPE_DEFAULTS:
				gtk_window_set_title (GTK_WINDOW (gconfwindow), _("Configuration Editor (Defaults settings)"));
				break;
			case GCONF_EDITOR_WINDOW_TYPE_MANDATORY:
				gtk_window_set_title (GTK_WINDOW (gconfwindow), _("Configuration Editor (Mandatory settings)"));
				break;
			default:
				gtk_window_set_title (GTK_WINDOW (gconfwindow), _("Configuration Editor"));
			}
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
		break;
	}
}