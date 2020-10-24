uint32_t  format_transportids(struct prout_param_descriptor *paramp)
{
	int i = 0, len;	
	uint32_t buff_offset = 4;
	memset(paramp->private_buffer, 0, MPATH_MAX_PARAM_LEN);
	for (i=0; i < paramp->num_transportid; i++ )
	{
		paramp->private_buffer[buff_offset] = (uint8_t)((paramp->trnptid_list[i]->format_code & 0xff)|
							(paramp->trnptid_list[i]->protocol_id & 0xff));
		buff_offset += 1;
		switch(paramp->trnptid_list[i]->protocol_id)
		{
			case MPATH_PROTOCOL_ID_FC:
				buff_offset += 7;
				memcpy(&paramp->private_buffer[buff_offset], &paramp->trnptid_list[i]->n_port_name, 8);
				buff_offset +=8 ;
				buff_offset +=8 ;
				break;
			case MPATH_PROTOCOL_ID_SAS:
				buff_offset += 3;
				memcpy(&paramp->private_buffer[buff_offset], &paramp->trnptid_list[i]->sas_address, 8);
				buff_offset += 12;
				break;
			case MPATH_PROTOCOL_ID_ISCSI:
				buff_offset += 1;
				len = (paramp->trnptid_list[i]->iscsi_name[1] & 0xff)+2;	
				memcpy(&paramp->private_buffer[buff_offset], &paramp->trnptid_list[i]->iscsi_name,len);
				buff_offset += len ; 
				break;
		}

	}
	buff_offset -= 4; 
	paramp->private_buffer[0] = (unsigned char)((buff_offset >> 24) & 0xff);
	paramp->private_buffer[1] = (unsigned char)((buff_offset >> 16) & 0xff);
	paramp->private_buffer[2] = (unsigned char)((buff_offset >> 8) & 0xff);
	paramp->private_buffer[3] = (unsigned char)(buff_offset & 0xff);
	buff_offset += 4; 
	return buff_offset;	
}
