snprint_blacklist_except (char * buff, int len)
{
	int i;
	struct blentry * ele;
	struct blentry_device * eled;
	int fwd = 0;
	struct keyword *rootkw;
	struct keyword *kw;

	rootkw = find_keyword(NULL, "blacklist_exceptions");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "blacklist_exceptions {\n");
	if (fwd > len)
		return len;

	vector_foreach_slot (conf->elist_devnode, ele, i) {
		kw = find_keyword(rootkw->sub, "devnode");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ele);
		if (fwd > len)
			return len;
	}
	vector_foreach_slot (conf->elist_wwid, ele, i) {
		kw = find_keyword(rootkw->sub, "wwid");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ele);
		if (fwd > len)
			return len;
	}
	vector_foreach_slot (conf->elist_property, ele, i) {
		kw = find_keyword(rootkw->sub, "property");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				       kw, ele);
		if (fwd > len)
			return len;
	}
	rootkw = find_keyword(rootkw->sub, "device");
	if (!rootkw)
		return 0;

	vector_foreach_slot (conf->elist_device, eled, i) {
		fwd += snprintf(buff + fwd, len - fwd, "\tdevice {\n");
		if (fwd > len)
			return len;
		kw = find_keyword(rootkw->sub, "vendor");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				       kw, eled);
		if (fwd > len)
			return len;
		kw = find_keyword(rootkw->sub, "product");
		if (!kw)
			return 0;
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				       kw, eled);
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
