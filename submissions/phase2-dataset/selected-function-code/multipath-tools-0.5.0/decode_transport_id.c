decode_transport_id(struct prin_fulldescr *fdesc, unsigned char * p, int length)
{
	int num, k;
	int jump;
	for (k = 0, jump = 24; k < length; k += jump, p += jump) {
		fdesc->trnptid.format_code = ((p[0] >> 6) & 0x3);
		fdesc->trnptid.protocol_id = (p[0] & 0xf);
		switch (fdesc->trnptid.protocol_id) {
			case MPATH_PROTOCOL_ID_FC:
				memcpy(&fdesc->trnptid.n_port_name, &p[8], 8);
				jump = 24;
				break;
			case MPATH_PROTOCOL_ID_ISCSI:
				num = ((p[2] << 8) | p[3]);
				memcpy(&fdesc->trnptid.iscsi_name, &p[4], num);
				jump = (((num + 4) < 24) ? 24 : num + 4);
				break;
			case MPATH_PROTOCOL_ID_SAS:
				memcpy(&fdesc->trnptid.sas_address, &p[4], 8);
				jump = 24;
				break;
			default:
				jump = 24;
				break;
		}
	}
}
