const char *osm_routing_engine_type_str(IN osm_routing_engine_type_t type)
{
	switch (type) {
	case OSM_ROUTING_ENGINE_TYPE_NONE:
		return "none";
	case OSM_ROUTING_ENGINE_TYPE_MINHOP:
		return "minhop";
	case OSM_ROUTING_ENGINE_TYPE_UPDN:
		return "updn";
	case OSM_ROUTING_ENGINE_TYPE_DNUP:
		return "dnup";
	case OSM_ROUTING_ENGINE_TYPE_FILE:
		return "file";
	case OSM_ROUTING_ENGINE_TYPE_FTREE:
		return "ftree";
	case OSM_ROUTING_ENGINE_TYPE_LASH:
		return "lash";
	case OSM_ROUTING_ENGINE_TYPE_DOR:
		return "dor";
	case OSM_ROUTING_ENGINE_TYPE_TORUS_2QOS:
		return "torus-2QoS";
	case OSM_ROUTING_ENGINE_TYPE_DFSSSP:
		return "dfsssp";
	case OSM_ROUTING_ENGINE_TYPE_SSSP:
		return "sssp";
	default:
		break;
	}
	return "unknown";
}