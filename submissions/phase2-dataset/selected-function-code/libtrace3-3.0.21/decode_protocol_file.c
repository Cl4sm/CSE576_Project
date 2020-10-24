void decode_protocol_file(uint16_t link_type UNUSED,const char *packet,int len,element_t *el)
{
    bitbuffer_t result;

    while(el != NULL)
    {
	switch(el->type)
	{
	    case FIELD:
	    	if (len*8+bits<el->data->field->size) {
			printf(" [Truncated]\n");
			return;
		}
		result = getbit((void*)&packet, &len, el->data->field->size); 

		switch(el->data->field->display)
		{
		    /* integers get byteswapped if needed and displayed */
		    case DISPLAY_INT: 
		    {
			result = fix_byteorder(result, 
				el->data->field->order, 
				el->data->field->size);
				
			el->data->field->value = result;
			printf(" %s %" PRIi64 "\n", 
				el->data->field->identifier,
				result);
		    }
		    break;

		    /* 
		     * hex numbers get byteswapped if needed and displayed 
		     * without being padded with zeroes
		     */
		    case DISPLAY_HEX: 
		    { 
			result = fix_byteorder(result, 
				el->data->field->order, 
				el->data->field->size);
			
			el->data->field->value = result;
			printf(" %s 0x%" PRIx64 "\n", 
				el->data->field->identifier,
				result);
		    }
		    break;
		    
		    /* 
		     * ipv4 addresses stay in network byte order and are
		     * given to inet_ntoa() to deal with
		     */
		    case DISPLAY_IPV4: 
		    {
			/* assumes all ipv4 addresses are 32bit fields */
			struct in_addr address;
			address.s_addr = (uint32_t)result;
			el->data->field->value = result;
		    
			printf(" %s %s\n", 
				el->data->field->identifier,
				inet_ntoa(address));
		    }
		    break;

		    /* 
		     * mac addresses stay in network byte order and are
		     * displayed byte by byte with zero padding
		     */
		    case DISPLAY_MAC: 
		    {
			/* assumes all mac addresses are 48bit fields */
			uint8_t *ptr = (uint8_t*)&result;
			el->data->field->value = result;
			printf(" %s %02x:%02x:%02x:%02x:%02x:%02x\n",
				el->data->field->identifier,
				ptr[0], ptr[1], ptr[2], 
				ptr[3], ptr[4], ptr[5]);
		    }
		    break;
		    
		    /*
		     * Flag values are only displayed if their value is true
		     * otherwise they are ignored
		     */
		    case DISPLAY_FLAG: 
		    {
			el->data->field->value = result;
			if(result)
			    printf(" %s\n", el->data->field->identifier);
		    }
		    break;

		    /*
		     * Hidden values are not displayed at all. This is useful
		     * for reserved fields or information that you don't care
		     * about but need to read in order to get to the rest of
		     * the header
		     */
		    case DISPLAY_NONE: 
		    {
			result = fix_byteorder(result, 
				el->data->field->order, 
				el->data->field->size);
			el->data->field->value = result;
		    }
		    break;
		};

		break;

	    case NEXTHEADER:
		/* 
		 * Before we move on to the next header, make sure our packet
		 * pointer is pointing to the first unused bytes. This may
		 * mean we have to backtrack to some that were put into the
		 * buffer but weren't used.
		 * - This wouldn't be a problem if all future output came
		 * from this buffer, but there is a good chance we will use
		 * some code from a shared library to output packet info
		 * instead, and this doesn't have access to the buffer.
		 */
		packet = packet - (bits / 8);
		len = len + (bits / 8);
		bits = 0;
		buffer = 0;

		decode_next(packet, len, el->data->nextheader->prefix, 
			ntohs(el->data->nextheader->target->value));
		break;
	};
	
	el = el->next;
    }
    buffer = 0;
    bits = 0;

}