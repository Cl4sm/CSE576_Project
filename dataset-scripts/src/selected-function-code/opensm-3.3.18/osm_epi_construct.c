osm_epi_plugin_t *osm_epi_construct(osm_opensm_t *osm, char *plugin_name)
{
	char lib_name[OSM_PATH_MAX];
	struct old_if { unsigned ver; } *old_impl;
	osm_epi_plugin_t *rc = NULL;

	if (!plugin_name || !*plugin_name)
		return NULL;

	/* find the plugin */
	snprintf(lib_name, sizeof(lib_name), "lib%s.so", plugin_name);

	rc = malloc(sizeof(*rc));
	if (!rc)
		return NULL;

	rc->handle = dlopen(lib_name, RTLD_LAZY);
	if (!rc->handle) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"Failed to open event plugin \"%s\" : \"%s\"\n",
			lib_name, dlerror());
		goto DLOPENFAIL;
	}

	rc->impl =
	    (osm_event_plugin_t *) dlsym(rc->handle,
					 OSM_EVENT_PLUGIN_IMPL_NAME);
	if (!rc->impl) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"Failed to find \"%s\" symbol in \"%s\" : \"%s\"\n",
			OSM_EVENT_PLUGIN_IMPL_NAME, lib_name, dlerror());
		goto Exit;
	}

	/* check for old interface */
	old_impl = (struct old_if *) rc->impl;
	if (old_impl->ver == OSM_ORIG_EVENT_PLUGIN_INTERFACE_VER) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR, "Error loading plugin: "
			"\'%s\' contains a depricated interface version %d\n"
			"   Please recompile with the new interface.\n",
			plugin_name, old_impl->ver);
		goto Exit;
	}

	/* Check the version to make sure this module will work with us */
	if (strcmp(rc->impl->osm_version, osm->osm_version)) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR, "Error loading plugin"
			" \'%s\': OpenSM version mismatch - plugin was built"
			" against %s version of OpenSM. Skip loading.\n",
			plugin_name, rc->impl->osm_version);
		goto Exit;
	}

	if (!rc->impl->create) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"Error loading plugin \'%s\': no create() method.\n",
			plugin_name);
		goto Exit;
	}

	rc->plugin_data = rc->impl->create(osm);

	if (!rc->plugin_data)
		goto Exit;

	rc->plugin_name = strdup(plugin_name);
	return rc;

Exit:
	dlclose(rc->handle);
DLOPENFAIL:
	free(rc);
	return NULL;
}