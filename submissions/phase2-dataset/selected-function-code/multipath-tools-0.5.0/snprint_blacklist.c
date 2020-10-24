snprint_blacklist (char * buff, int len)
{
	int i;
	struct blentry * ble;
	struct blentry_device * bled;
	int fwd = 0;
	struct keyword *rootkw;
	struct keyword *kw;

	rootkw = find_keyword(NULL, "blacklist");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "blacklist {\n");
	if (fwd > len)
		return len;

	vector_foreach_slot (conf->blist_devnode, ble, i) {
		kw = find_keyword(rootkw->sub, "devnode");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ble);
		if (fwd > len)
			return len;
	}
	vector_foreach_slot (conf->blist_wwid, ble, i) {
		kw = find_keyword(rootkw->sub, "wwid");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ble);
		if (fwd > len)
			return len;
	}
	vector_foreach_slot (conf->blist_property, ble, i) {
		kw = find_keyword(rootkw->sub, "property");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ble);
		if (fwd > len)
			return len;
	}
	rootkw = find_keyword(rootkw->sub, "device");
	if (!rootkw)
		return 0;

	vector_foreach_slot (conf->blist_device, bled, i) {
		fwd += snprintf(buff + fwd, len - fwd, "\tdevice {\n");
		if (fwd > len)
			return len;
		kw = find_keyword(rootkw->sub, "vendor");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				       kw, bled);
		if (fwd > len)
			return len;
		kw = find_keyword(rootkw->sub, "product");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				       kw, bled);
		if (fwd > len)
			return len;
		fwd += snprintf(buff + fwd, len - fwd, "\t}\n");
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "}\n");
	if (fwd > len)
		return len;
	return fwd;
}
