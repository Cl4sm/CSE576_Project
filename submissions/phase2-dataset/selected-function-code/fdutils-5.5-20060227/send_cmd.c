int send_cmd(int fd,struct floppy_raw_cmd *raw_cmd, char *message)
{
	int i;
	int code;
	int retries;
	struct floppy_raw_cmd tmp;
	
	retries=0;
	tmp = *raw_cmd;
 repeat:
	*raw_cmd = tmp;
	if ( ioctl( fd, FDRAWCMD, raw_cmd) < 0 ){
		if ( errno == EBUSY ){
			fprintf(stderr,"FDC busy, sleeping for a second\n");
			sleep(1);
			goto repeat;
		}
		perror(message);
#if 0
		printf("the final skew is %d\n", skew );
#endif
		exit(1);
	}

	if ( raw_cmd->reply_count ){
		switch( raw_cmd->reply[0] & 0xc0 ){
		case 0x40:
			/* ignore EOC */
			if((raw_cmd->reply[0] & ~0x4)== 0x40 &&
			   (raw_cmd->reply[1] == 0x80) &&
			   (raw_cmd->reply[2] == 0)) {
				/* OK, we merely reached the end of 
				 * our cylinder */
				break;
			}

			if((raw_cmd->reply[0] & ~0x4)== 0x40 &&
			   raw_cmd->reply[1] == 0x10 &&
			   raw_cmd->reply[2] == 0x00) {
				fprintf(stderr,"Overrun, pausing for a second\n");
				sleep(1);
				goto repeat;
			}

			fprintf(stderr,"error during command execution\n");
			if ( raw_cmd->reply[1] & ST1_WP ){
				fprintf(stderr,"The disk is write protected\n");
				exit(1);
			}
			fprintf(stderr,"   ");
			for (i=0; i< raw_cmd->cmd_count; i++)
				fprintf(stderr,"%2.2x ", (int)raw_cmd->cmd[i] );
			printf("\n");
			for (i=0; i< raw_cmd->reply_count; i++)
				fprintf(stderr,"%2.2x ", (int)raw_cmd->reply[i] );
			fprintf(stderr,"\n");
			code = (raw_cmd->reply[0] <<16) + 
				(raw_cmd->reply[1] << 8) + 
					raw_cmd->reply[2];
			for(i=0; i<22; i++){
				if ( (code & ( 1 << i )) && error_msg[i])
					fprintf(stderr,"%s\n", error_msg[i]);
			}
			printf("cylinder=%d head=%d sector=%d size=%d\n",
			       raw_cmd->reply[3],
			       raw_cmd->reply[4],
			       raw_cmd->reply[5],
			       raw_cmd->reply[6]);
			return -1;
			break;
		case 0x80:
			fprintf(stderr,"invalid command given\n");
			exit(1);
		case 0xc0:
			fprintf(stderr,"abnormal termination caused by polling\n");
			exit(1);
		case 0:
			/* OK */
			break;
		}
	}
	return 0;
}
