void print_result(struct floppy_raw_cmd *raw_cmd, long long date)
{
	int i;

	if ( do_short ){
		if ( raw_cmd->flags & ( FD_RAW_READ | FD_RAW_WRITE ))
			fprintf(f,"%lu ", raw_cmd->length );  
		for( i=0; i< raw_cmd->reply_count; i++ )
			fprintf(f,"0x%02x ", raw_cmd->reply[i] );
#ifdef FD_RAW_DISK_CHANGE
		if( raw_cmd->flags & FD_RAW_DISK_CHANGE)
			fprintf(f,"disk_change ");
		else
			fprintf(f,"no_disk_change ");
#endif
	} else {    
		if ( raw_cmd->flags & ( FD_RAW_READ | FD_RAW_WRITE ))
			fprintf(f,"remaining= %lu\n", raw_cmd->length );  
		for( i=0; i< raw_cmd->reply_count; i++ )
			fprintf(f,"%d: %x\n", i, raw_cmd->reply[i] );
#ifdef FD_RAW_DISK_CHANGE
		if( raw_cmd->flags & FD_RAW_DISK_CHANGE)
			fprintf(f,"disk change\n");
		else
			fprintf(f,"no disk change\n");
#endif
	}
	if ( print_time )
		fprintf(f,"%Ld\n", date);
	else
		fprintf(f,"\n");
}
