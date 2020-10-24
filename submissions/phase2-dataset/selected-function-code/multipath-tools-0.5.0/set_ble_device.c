extern int
set_ble_device (vector blist, char * vendor, char * product, int origin)
{
	struct blentry_device * ble;

	if (!blist)
		return 1;

	ble = VECTOR_LAST_SLOT(blist);

	if (!ble)
		return 1;

	if (vendor) {
		if (regcomp(&ble->vendor_reg, vendor,
			    REG_EXTENDED|REG_NOSUB)) {
			FREE(vendor);
			return 1;
		}
		ble->vendor = vendor;
	}
	if (product) {
		if (regcomp(&ble->product_reg, product,
			    REG_EXTENDED|REG_NOSUB)) {
			FREE(product);
			return 1;
		}
		ble->product = product;
	}
	ble->origin = origin;
	return 0;
}
