{
	sendip_data *data = NULL;
	u_int8_t    *ptr;
	
	data = malloc(sizeof(sendip_data));
	
	if (data != NULL) {
		memset(data, 0, sizeof(sendip_data));
		data->data = malloc(BGP_BUFLEN);
		if (data->data == NULL) {
			free(data);
			data = NULL;
		}
	}
	
	if (data != NULL) {
		memset(data->data, 0, BGP_BUFLEN);
		ptr = data->data;
		
		memset(data->data, 0xFF, 16);
		ptr += 16;
		bgp_len_ptr = ptr;
		PUTSHORT(ptr, 19);
		ptr += 2;
		*ptr++ = 4;
		
		data->alloc_len = ptr - (u_int8_t *)data->data;
		bgp_prev_part = BGP_HEADER;
	} 
	return (data);
}
