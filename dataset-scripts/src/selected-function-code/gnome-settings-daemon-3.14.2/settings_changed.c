static void
settings_changed (GSettings          *settings,
                  const char         *key,
                  GsdKeyboardManager *manager)
{
	if (g_strcmp0 (key, KEY_CLICK) == 0||
	    g_strcmp0 (key, KEY_CLICK_VOLUME) == 0 ||
	    g_strcmp0 (key, KEY_BELL_PITCH) == 0 ||
	    g_strcmp0 (key, KEY_BELL_DURATION) == 0 ||
	    g_strcmp0 (key, KEY_BELL_MODE) == 0) {
		g_debug ("Bell setting '%s' changed, applying bell settings", key);
		apply_bell (manager);
	} else if (g_strcmp0 (key, KEY_REMEMBER_NUMLOCK_STATE) == 0) {
		g_debug ("Remember Num-Lock state '%s' changed, applying num-lock settings", key);
		apply_numlock (manager);
	} else if (g_strcmp0 (key, KEY_NUMLOCK_STATE) == 0) {
		g_debug ("Num-Lock state '%s' changed, will apply at next startup", key);
	} else if (g_strcmp0 (key, KEY_REPEAT) == 0 ||
		 g_strcmp0 (key, KEY_INTERVAL) == 0 ||
		 g_strcmp0 (key, KEY_DELAY) == 0) {
		g_debug ("Key repeat setting '%s' changed, applying key repeat settings", key);
		apply_repeat (manager);
	} else if (g_strcmp0 (key, KEY_BELL_CUSTOM_FILE) == 0){
		g_debug ("Ignoring '%s' setting change", KEY_BELL_CUSTOM_FILE);
	} else {
		g_warning ("Unhandled settings change, key '%s'", key);
	}

}