static int erf_probe_magic(io_t *io)
{
	char buffer[4096];
	int len;
	dag_record_t *erf;
	len = wandio_peek(io, buffer, sizeof(buffer));
	if (len < (int)dag_record_size) {
		return 0; /* False */
	}
	erf = (dag_record_t *) buffer;
	/* If the record is too short */
	if (ntohs(erf->rlen) < dag_record_size) {
		return 0;
	}
	/* There aren't any erf traces before 1995-01-01 */
	if (bswap_le_to_host64(erf->ts) < 0x2f0539b000000000ULL) {
		return 0;
	}
	/* And not pcap! */
	if (bswap_le_to_host64(erf->ts) >>32 == 0xa1b2c3d4) {
		return 0;
	}
	/* And not the other pcap! */
	if (bswap_le_to_host64(erf->ts) >>32 == 0xd4c3b2a1) {
		return 0;
	}
	/* Is this a proper typed packet */
	if (erf->type > TYPE_AAL2) {
		return 0;
	}
	/* We should put some more tests in here. */
	/* Yeah, this is probably ERF */
	return 1;
}