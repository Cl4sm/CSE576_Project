                unsigned int rq_type, struct prout_param_descriptor *paramp, int noisy)
{

	int status, paramlen = 24, ret = 0;
	uint32_t translen=0;
	int retry = MAXRETRY;
	SenseData_t Sensedata;
	struct sg_io_hdr io_hdr;
	char devname[FILE_NAME_SIZE];
	int fd = -1;

	snprintf(devname, FILE_NAME_SIZE, "/dev/%s",dev);
	fd = open(devname, O_WRONLY);
	if(fd < 0){
		condlog (1, "%s: unable to open device.", dev);
		return MPATH_PR_FILE_ERROR;
	}

	unsigned char cdb[MPATH_PROUT_CMDLEN] =
	{MPATH_PROUT_CMD, 0, 0, 0, 0, 0, 0, 0, 0, 0};


	if (paramp->sa_flags & MPATH_F_SPEC_I_PT_MASK)
	{
		translen = format_transportids(paramp);
		paramlen = 24 + translen;
	}	
	else
		paramlen = 24;

	if ( rq_servact > 0)
		cdb[1] = (unsigned char)(rq_servact & 0x1f);
	cdb[2] = (((rq_scope & 0xf) << 4) | (rq_type & 0xf));
	cdb[7] = (unsigned char)((paramlen >> 8) & 0xff);
	cdb[8] = (unsigned char)(paramlen & 0xff);

retry :
	condlog(3, "%s: rq_servact = %d", dev, rq_servact); 
	condlog(3, "%s: rq_scope = %d ", dev, rq_scope);
	condlog(3, "%s: rq_type = %d ", dev, rq_type);
	condlog(3, "%s: paramlen = %d", dev, paramlen);

	if (noisy)
	{
		condlog(3, "%s: Persistent Reservation OUT parameter:", dev);
		dumpHex((const char *)paramp, paramlen,1);
	}

	memset(&Sensedata, 0, sizeof(SenseData_t));
	memset(&io_hdr,0 , sizeof( struct sg_io_hdr));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = MPATH_PROUT_CMDLEN;
	io_hdr.cmdp = cdb;
	io_hdr.sbp = (void *)&Sensedata;
	io_hdr.mx_sb_len = sizeof (SenseData_t);
	io_hdr.timeout = TIMEOUT;

	if (paramlen > 0) {
		io_hdr.dxferp = (void *)paramp;
		io_hdr.dxfer_len = paramlen;
		io_hdr.dxfer_direction = SG_DXFER_TO_DEV ;
	}
	else {
		io_hdr.dxfer_direction = SG_DXFER_NONE;
	}
	ret = ioctl(fd, SG_IO, &io_hdr);
	if (ret < 0)
	{
		condlog(0, "%s: ioctl failed %d", dev, ret);
		close(fd);
		return ret;
	}

	condlog(2, "%s: Duration=%u (ms)", dev, io_hdr.duration);

	status = mpath_translate_response(dev, io_hdr, Sensedata, noisy);
	condlog(3, "%s: status = %d", dev, status);

	if (status == MPATH_PR_SENSE_UNIT_ATTENTION && (retry > 0))
	{
		--retry;
		condlog(2, "%s: retrying for Unit Attention. Remaining retries = %d", 
			dev, retry);
		goto retry;
	}

	if (((status == MPATH_PR_SENSE_NOT_READY )&& (Sensedata.ASC == 0x04)&&
				(Sensedata.ASCQ == 0x07))&& (retry > 0))
	{
		usleep(1000);
		--retry;
		condlog(2, "%s: retrying for sense 02/04/07."
			" Remaining retries = %d", dev, retry);
		goto retry;
	}

	close(fd);	
	return status;
}
