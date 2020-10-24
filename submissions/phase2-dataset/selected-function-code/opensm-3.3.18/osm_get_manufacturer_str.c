const char *osm_get_manufacturer_str(IN uint64_t guid_ho)
{
	/* note that the max vendor string length is 11 */
	static const char *intel_str = "Intel";
	static const char *mellanox_str = "Mellanox";
	static const char *redswitch_str = "Redswitch";
	static const char *silverstorm_str = "SilverStorm";
	static const char *topspin_str = "Topspin";
	static const char *fujitsu_str = "Fujitsu";
	static const char *voltaire_str = "Voltaire";
	static const char *yotta_str = "YottaYotta";
	static const char *pathscale_str = "PathScale";
	static const char *ibm_str = "IBM";
	static const char *divergenet_str = "DivergeNet";
	static const char *flextronics_str = "Flextronics";
	static const char *agilent_str = "Agilent";
	static const char *obsidian_str = "Obsidian";
	static const char *baymicro_str = "BayMicro";
	static const char *lsilogic_str = "LSILogic";
	static const char *ddn_str = "DataDirect";
	static const char *panta_str = "Panta";
	static const char *hp_str = "HP";
	static const char *rioworks_str = "Rioworks";
	static const char *sun_str = "Sun";
	static const char *leafntwks_str = "3LeafNtwks";
	static const char *xsigo_str = "Xsigo";
	static const char *dell_str = "Dell";
	static const char *supermicro_str = "SuperMicro";
	static const char *openib_str = "OpenIB";
	static const char *unknown_str = "Unknown";

	switch ((uint32_t) (guid_ho >> (5 * 8))) {
	case OSM_VENDOR_ID_INTEL:
		return intel_str;
	case OSM_VENDOR_ID_MELLANOX:
	case OSM_VENDOR_ID_MELLANOX2:
	case OSM_VENDOR_ID_MELLANOX3:
	case OSM_VENDOR_ID_MELLANOX4:
		return mellanox_str;
	case OSM_VENDOR_ID_REDSWITCH:
		return redswitch_str;
	case OSM_VENDOR_ID_SILVERSTORM:
		return silverstorm_str;
	case OSM_VENDOR_ID_TOPSPIN:
		return topspin_str;
	case OSM_VENDOR_ID_FUJITSU:
	case OSM_VENDOR_ID_FUJITSU2:
		return fujitsu_str;
	case OSM_VENDOR_ID_VOLTAIRE:
		return voltaire_str;
	case OSM_VENDOR_ID_YOTTAYOTTA:
		return yotta_str;
	case OSM_VENDOR_ID_PATHSCALE:
		return pathscale_str;
	case OSM_VENDOR_ID_IBM:
	case OSM_VENDOR_ID_IBM2:
		return ibm_str;
	case OSM_VENDOR_ID_DIVERGENET:
		return divergenet_str;
	case OSM_VENDOR_ID_FLEXTRONICS:
		return flextronics_str;
	case OSM_VENDOR_ID_AGILENT:
		return agilent_str;
	case OSM_VENDOR_ID_OBSIDIAN:
		return obsidian_str;
	case OSM_VENDOR_ID_BAYMICRO:
		return baymicro_str;
	case OSM_VENDOR_ID_LSILOGIC:
		return lsilogic_str;
	case OSM_VENDOR_ID_DDN:
		return ddn_str;
	case OSM_VENDOR_ID_PANTA:
		return panta_str;
	case OSM_VENDOR_ID_HP:
	case OSM_VENDOR_ID_HP2:
	case OSM_VENDOR_ID_HP3:
	case OSM_VENDOR_ID_HP4:
		return hp_str;
	case OSM_VENDOR_ID_RIOWORKS:
		return rioworks_str;
	case OSM_VENDOR_ID_SUN:
	case OSM_VENDOR_ID_SUN2:
		return sun_str;
	case OSM_VENDOR_ID_3LEAFNTWKS:
		return leafntwks_str;
	case OSM_VENDOR_ID_XSIGO:
		return xsigo_str;
	case OSM_VENDOR_ID_DELL:
		return dell_str;
	case OSM_VENDOR_ID_SUPERMICRO:
		return supermicro_str;
	case OSM_VENDOR_ID_OPENIB:
		return openib_str;
	default:
		return unknown_str;
	}
}