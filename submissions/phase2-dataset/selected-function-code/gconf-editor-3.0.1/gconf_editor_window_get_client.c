static GConfClient*
gconf_editor_window_get_client (int type)
{
	GConfEngine *engine;
	GConfClient *client = NULL;
	GError *error = NULL;

	switch (type) {
	case GCONF_EDITOR_WINDOW_TYPE_NORMAL:
		client = gconf_client_get_default ();
		break;
	case GCONF_EDITOR_WINDOW_TYPE_DEFAULTS:
		engine = gconf_engine_get_for_address (GCONF_DEFAULTS_SOURCE, &error);
		if (error) {
			gconf_editor_window_popup_error_dialog (NULL,
			_("Cannot create GConf engine. Error was:\n%s"), error);
			return NULL;
		}
		client = gconf_client_get_for_engine (engine);
		gconf_engine_unref (engine);
		break;
	case GCONF_EDITOR_WINDOW_TYPE_MANDATORY:
		engine = gconf_engine_get_for_address (GCONF_MANDATORY_SOURCE, &error);
		if (error) {
			gconf_editor_window_popup_error_dialog (NULL,
			_("Cannot create GConf engine. Error was:\n%s"), error);
			return NULL;
		}
		client = gconf_client_get_for_engine (engine);
		gconf_engine_unref (engine);
		break;
	default:
		g_assert_not_reached ();
	}

	return client;
}