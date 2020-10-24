void mpath_format_readfullstatus(struct prin_resp *pr_buff, int len, int noisy)
{
	int num, k, tid_len_len=0;
	uint32_t fdesc_count=0;
	unsigned char *p;
	char  *ppbuff;
	uint32_t additional_length;


	mpath_reverse_uint32_byteorder(&pr_buff->prin_descriptor.prin_readfd.prgeneration);
	mpath_reverse_uint32_byteorder(&pr_buff->prin_descriptor.prin_readfd.number_of_descriptor);

	if (0 == pr_buff->prin_descriptor.prin_readfd.number_of_descriptor)
	{
		return ;
	}


	if (pr_buff->prin_descriptor.prin_readfd.number_of_descriptor == 0)
	{
		condlog(2, "No registration or resrvation found.");
		return;
	}

	additional_length = pr_buff->prin_descriptor.prin_readfd.number_of_descriptor;

	char tempbuff[MPATH_MAX_PARAM_LEN];
	struct prin_fulldescr fdesc;
	memset(&fdesc, 0, sizeof(struct prin_fulldescr));

	memcpy( tempbuff, pr_buff->prin_descriptor.prin_readfd.private_buffer,MPATH_MAX_PARAM_LEN );
	memset(&pr_buff->prin_descriptor.prin_readfd.private_buffer, 0, MPATH_MAX_PARAM_LEN);

	p =(unsigned char *)tempbuff;
	ppbuff = (char *)pr_buff->prin_descriptor.prin_readfd.private_buffer;

	for (k = 0; k < additional_length; k += num, p += num) {
		memcpy(&fdesc.key, p, 8 );
		fdesc.flag = p[12];
		fdesc.scope_type =  p[13];
		fdesc.rtpi = ((p[18] << 8) | p[19]);

		tid_len_len = ((p[20] << 24) | (p[21] << 16) |
				(p[22] << 8) | p[23]);

		if (tid_len_len > 0)
			decode_transport_id( &fdesc, &p[24], tid_len_len);

		num = 24 + tid_len_len;
		memcpy(ppbuff, &fdesc, sizeof(struct prin_fulldescr));
		pr_buff->prin_descriptor.prin_readfd.descriptors[fdesc_count]= (struct prin_fulldescr *)ppbuff;
		ppbuff += sizeof(struct prin_fulldescr);
		++fdesc_count;
	}

	pr_buff->prin_descriptor.prin_readfd.number_of_descriptor = fdesc_count;

	return;
}
