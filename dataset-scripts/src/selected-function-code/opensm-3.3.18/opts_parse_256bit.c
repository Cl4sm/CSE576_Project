static void opts_parse_256bit(IN osm_subn_t *p_subn, IN char *p_key,
			      IN char *p_val_str, void *p_v1, void *p_v2,
			      void (*pfn)(osm_subn_t *, void *))
{
	uint8_t *p_val1 = p_v1, *p_val2 = p_v2;
	uint8_t val[IB_CC_PORT_MASK_DATA_SIZE] = { 0 };
	char tmpbuf[3] = { 0 };
	uint8_t tmpint;
	int numdigits = 0;
	int startindex;
	char *strptr = p_val_str;
	char *ptr;
	int i;

	/* parse like it's hypothetically a 256 bit integer code
	 *
	 * store "big endian"
	 */

	if (!strncmp(strptr, "0x", 2) || !strncmp(strptr, "0X", 2))
		strptr+=2;

	for (ptr = strptr; *ptr; ptr++) {
		if (!isxdigit(*ptr)) {
			log_report("invalid hex digit in bitmask\n");
			return;
		}
		numdigits++;
	}

	if (!numdigits) {
		log_report("invalid length bitmask\n");
		return;
	}

	/* max of 2 hex chars per byte */
	if (numdigits > IB_CC_PORT_MASK_DATA_SIZE * 2)
		numdigits = IB_CC_PORT_MASK_DATA_SIZE * 2;

	startindex = IB_CC_PORT_MASK_DATA_SIZE - ((numdigits - 1) / 2) - 1;

	if (numdigits % 2) {
		memcpy(tmpbuf, strptr, 1);
		strptr += 1;
	}
	else {
		memcpy(tmpbuf, strptr, 2);
		strptr += 2;
	}

	tmpint = strtoul(tmpbuf, NULL, 16);
	val[startindex] = tmpint;

	for (i = (startindex + 1); i < IB_CC_PORT_MASK_DATA_SIZE; i++) {
		memcpy(tmpbuf, strptr, 2);
		strptr += 2;
		tmpint = strtoul(tmpbuf, NULL, 16);
		val[i] = tmpint;
	}

	if (memcmp(val, p_val1, IB_CC_PORT_MASK_DATA_SIZE)) {
		log_config_value(p_key, "%s", p_val_str);
		if (pfn)
			pfn(p_subn, val);
		memcpy(p_val1, val, IB_CC_PORT_MASK_DATA_SIZE);
		memcpy(p_val2, val, IB_CC_PORT_MASK_DATA_SIZE);
	}

}