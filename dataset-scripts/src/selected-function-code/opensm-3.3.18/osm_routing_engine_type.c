osm_routing_engine_type_t osm_routing_engine_type(IN const char *str)
{
	/* For legacy reasons, consider a NULL pointer and the string
	 * "null" as the minhop routing engine.
	 */
	if (!str || !strcasecmp(str, "null")
	    || !strcasecmp(str, "minhop"))
		return OSM_ROUTING_ENGINE_TYPE_MINHOP;
	else if (!strcasecmp(str, "none"))
		return OSM_ROUTING_ENGINE_TYPE_NONE;
	else if (!strcasecmp(str, "updn"))
		return OSM_ROUTING_ENGINE_TYPE_UPDN;
	else if (!strcasecmp(str, "dnup"))
		return OSM_ROUTING_ENGINE_TYPE_DNUP;
	else if (!strcasecmp(str, "file"))
		return OSM_ROUTING_ENGINE_TYPE_FILE;
	else if (!strcasecmp(str, "ftree"))
		return OSM_ROUTING_ENGINE_TYPE_FTREE;
	else if (!strcasecmp(str, "lash"))
		return OSM_ROUTING_ENGINE_TYPE_LASH;
	else if (!strcasecmp(str, "dor"))
		return OSM_ROUTING_ENGINE_TYPE_DOR;
	else if (!strcasecmp(str, "torus-2QoS"))
		return OSM_ROUTING_ENGINE_TYPE_TORUS_2QOS;
	else if (!strcasecmp(str, "sssp"))
		return OSM_ROUTING_ENGINE_TYPE_SSSP;
	else if (!strcasecmp(str, "dfsssp"))
		return OSM_ROUTING_ENGINE_TYPE_DFSSSP;
	else
		return OSM_ROUTING_ENGINE_TYPE_UNKNOWN;
}