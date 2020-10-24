void
free_blacklist_device (vector blist)
{
	struct blentry_device * ble;
	int i;

	if (!blist)
		return;

	vector_foreach_slot (blist, ble, i) {
		if (ble) {
			if (ble->vendor) {
				regfree(&ble->vendor_reg);
				FREE(ble->vendor);
			}
			if (ble->product) {
				regfree(&ble->product_reg);
				FREE(ble->product);
			}
			FREE(ble);
		}
	}
	vector_free(blist);
}
