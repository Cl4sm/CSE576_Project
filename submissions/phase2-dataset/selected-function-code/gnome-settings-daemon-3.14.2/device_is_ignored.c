static gboolean
device_is_ignored (GsdMouseManager *manager,
		   GdkDevice       *device)
{
	GdkInputSource source;
	const char *name;

	if (device_is_blacklisted (manager, device))
		return TRUE;

	source = gdk_device_get_source (device);
	if (source != GDK_SOURCE_MOUSE &&
	    source != GDK_SOURCE_TOUCHPAD &&
	    source != GDK_SOURCE_CURSOR)
		return TRUE;

	name = gdk_device_get_name (device);
	if (name != NULL && g_str_equal ("Virtual core XTEST pointer", name))
		return TRUE;

	return FALSE;
}