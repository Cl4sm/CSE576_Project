mpath_print_transport_id(struct prin_fulldescr *fdesc)
{
	switch (fdesc->trnptid.protocol_id) {
		case MPATH_PROTOCOL_ID_FC:
			printf("   FCP-2 ");
			if (0 != fdesc->trnptid.format_code)
				printf(" [Unexpected format code: %d]\n", 
						fdesc->trnptid.format_code);
			dumpHex((const char *)fdesc->trnptid.n_port_name, 8, 0);
			break;
		case MPATH_PROTOCOL_ID_ISCSI:
			printf("   iSCSI ");
			if (0 == fdesc->trnptid.format_code) {
				printf("name: %.*s\n", (int)sizeof(fdesc->trnptid.iscsi_name),
					fdesc->trnptid.iscsi_name);
			}else if (1 == fdesc->trnptid.format_code){
				printf("world wide unique port id: %.*s\n",
						(int)sizeof(fdesc->trnptid.iscsi_name),
						fdesc->trnptid.iscsi_name);
			}else {
				printf("  [Unexpected format code: %d]\n", fdesc->trnptid.format_code);
				dumpHex((const char *)fdesc->trnptid.iscsi_name,
					 (int)sizeof(fdesc->trnptid.iscsi_name), 0);
			}
			break;
		case MPATH_PROTOCOL_ID_SAS:
			printf("   SAS ");
			 if (0 != fdesc->trnptid.format_code)
                                printf(" [Unexpected format code: %d]\n",
                                                fdesc->trnptid.format_code);
                        dumpHex((const char *)fdesc->trnptid.sas_address, 8, 0);
			break;
		default:
			return;
	}
}
