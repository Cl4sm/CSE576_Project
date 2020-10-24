{
	int i,k;
	int code;

	k = 0;
	do {
		if ( raw_cmd[k].reply_count ){
			switch( raw_cmd[k].reply[0] & 0xc0 ){
				case 0x40:
					if((raw_cmd[k].reply[0] & 0x38) == 0 &&
					   raw_cmd[k].reply[1] == 0x80 &&
					   raw_cmd[k].reply[2] == 0)
						break;

					if(probe_only)
						return -2;
					curcylinder = -1;
					fprintf(stderr,
						"\nerror during command execution\n");
					if ( raw_cmd[k].reply[1] & ST1_WP ){
						fprintf(stderr,
							"The disk is write protected\n");
						exit(1);
					}
					fprintf(stderr,"   ");
					for (i=0; i< raw_cmd[k].cmd_count; i++)
						fprintf(stderr,"%2.2x ", 
							(int)raw_cmd[k].cmd[i] );
					printf("\n");
					for (i=0; i< raw_cmd[k].reply_count; i++)
						fprintf(stderr,"%2.2x ", 
							(int)raw_cmd[k].reply[i] );
					fprintf(stderr,"\n");
					code = (raw_cmd[k].reply[0] <<16) + 
						(raw_cmd[k].reply[1] << 8) + 
						raw_cmd[k].reply[2];
					for(i=0; i<22; i++){
					if ( (code & ( 1 << i )) && 
					     error_msg[i])
						fprintf(stderr, "%s\n",
							error_msg[i]);
					}
					sleep(4);
					return k;
				case 0x80:
					curcylinder = -1;
					fprintf(stderr,
						"\ninvalid command given\n");
					return 1;
				case 0xc0:
					curcylinder = -1;
					fprintf(stderr,
						"\nabnormal termination caused by polling\n");
					return 0;
				case 0:
					break;
			}
			if (raw_cmd[k].flags & FD_RAW_NEED_SEEK)
			    curcylinder = raw_cmd[k].track;
			/* OK */			
		} else {
			fprintf(stderr,"\nNull reply from FDC\n");
			return 1;
		}
		k++;
	} while(raw_cmd[k-1].flags & FD_RAW_MORE);
	return -1;
}
