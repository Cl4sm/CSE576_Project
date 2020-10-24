static boolean_t validate_requested_mgid(IN osm_sa_t * sa,
					 IN const ib_member_rec_t * p_mcm_rec)
{
	uint16_t signature;
	boolean_t valid = TRUE;

	OSM_LOG_ENTER(sa->p_log);

	/* 14-a: mcast GID must start with 0xFF */
	if (p_mcm_rec->mgid.multicast.header[0] != 0xFF) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B01: "
			"Invalid prefix 0x%02X in requested MGID, "
			"must be 0xFF\n",
			cl_ntoh16(p_mcm_rec->mgid.multicast.header[0]));
		valid = FALSE;
		goto Exit;
	}

	/* the MGID signature can mark IPoIB or SA assigned MGIDs */
	memcpy(&signature, &(p_mcm_rec->mgid.multicast.raw_group_id),
	       sizeof(signature));
	signature = cl_ntoh16(signature);
	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "MGID Signed as 0x%04X\n", signature);

	/*
	 * We skip any checks for MGIDs that follow IPoIB
	 * GID structure as defined by the IETF ipoib-link-multicast.
	 *
	 * For IPv4 over IB, the signature will be "0x401B".
	 *
	 * |   8    |  4 |  4 |     16 bits     | 16 bits | 48 bits  | 32 bits |
	 * +--------+----+----+-----------------+---------+----------+---------+
	 * |11111111|0001|scop|<IPoIB signature>|< P_Key >|00.......0|<all 1's>|
	 * +--------+----+----+-----------------+---------+----------+---------+
	 *
	 * For IPv6 over IB, the signature will be "0x601B".
	 *
	 * |   8    |  4 |  4 |     16 bits     | 16 bits |       80 bits      |
	 * +--------+----+----+-----------------+---------+--------------------+
	 * |11111111|0001|scop|<IPoIB signature>|< P_Key >|000.............0001|
	 * +--------+----+----+-----------------+---------+--------------------+
	 *
	 */
	if (signature == 0x401B || signature == 0x601B) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Skipping MGID Validation for IPoIB Signed (0x%04X) MGIDs\n",
			signature);
		goto Exit;
	}

	/* 14-b: the 3 upper bits in the "flags" should be zero: */
	if (p_mcm_rec->mgid.multicast.header[1] & 0xE0) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B28: "
			"Requested MGID invalid, uses Reserved Flags: flags=0x%X\n",
			(p_mcm_rec->mgid.multicast.header[1] & 0xE0) >> 4);
		valid = FALSE;
		goto Exit;
	}

	/* 2 - now what if the link local format 0xA01B is used -
	   the scope should not be link local */
	if (signature == 0xA01B &&
	    (p_mcm_rec->mgid.multicast.header[1] & 0x0F) ==
	    IB_MC_SCOPE_LINK_LOCAL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B24: "
			"Requested MGID invalid, "
			"uses 0xA01B signature but with link-local scope\n");
		valid = FALSE;
		goto Exit;
	}

	/*
	 * For SA assigned MGIDs (signature 0xA01B):
	 * There is no real way to make sure the GID Prefix is really unique.
	 * If we could enforce using the Subnet Prefix for that purpose it would
	 * have been nice. But the spec does not require it.
	 */

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return valid;
}