log_filter (const char *dev, char *vendor, char *product, char *wwid,
	    const char *env, int r)
{
	/*
	 * Try to sort from most likely to least.
	 */
	switch (r) {
	case MATCH_NOTHING:
		break;
	case MATCH_DEVICE_BLIST:
		LOG_BLIST("vendor/product", "blacklisted");
		break;
	case MATCH_WWID_BLIST:
		LOG_BLIST("wwid", "blacklisted");
		break;
	case MATCH_DEVNODE_BLIST:
		LOG_BLIST("device node name", "blacklisted");
		break;
	case MATCH_PROPERTY_BLIST:
		LOG_BLIST("udev property", "blacklisted");
		break;
	case MATCH_DEVICE_BLIST_EXCEPT:
		LOG_BLIST("vendor/product", "whitelisted");
		break;
	case MATCH_WWID_BLIST_EXCEPT:
		LOG_BLIST("wwid", "whitelisted");
		break;
	case MATCH_DEVNODE_BLIST_EXCEPT:
		LOG_BLIST("device node name", "whitelisted");
		break;
	case MATCH_PROPERTY_BLIST_EXCEPT:
		LOG_BLIST("udev property", "whitelisted");
		break;
	case MATCH_PROPERTY_BLIST_MISSING:
		LOG_BLIST("blacklisted,", "udev property missing");
		break;
	}
}
