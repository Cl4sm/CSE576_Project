get_transportids_length(unsigned char * transportid_arr, int max_transportid, int num_transportids)
{
	int compact_len = 0;
	unsigned char * ucp = transportid_arr;
	int k, off, protocol_id, len;
	for (k = 0, off = 0; ((k < num_transportids) && (k < max_transportid));
			++k, off += MPATH_MX_TID_LEN) {
		protocol_id = ucp[off] & 0xf;
		if (5 == protocol_id) {
			len = (ucp[off + 2] << 8) + ucp[off + 3] + 4;
			if (len < 24)
				len = 24;
			if (off > compact_len)
				memmove(ucp + compact_len, ucp + off, len);
			compact_len += len;

		} else {
			if (off > compact_len)
				memmove(ucp + compact_len, ucp + off, 24);
			compact_len += 24;
		}
	}

	return compact_len;
}
