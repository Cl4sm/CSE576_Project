{

	int ret, status, got, fd;
	int mx_resp_len;
	SenseData_t Sensedata;
	int retry = MAXRETRY;
	struct sg_io_hdr io_hdr;
	char devname[FILE_NAME_SIZE];
	unsigned char cdb[MPATH_PRIN_CMDLEN] =
	{MPATH_PRIN_CMD, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	snprintf(devname, FILE_NAME_SIZE, "/dev/%s",dev);
        fd = open(devname, O_WRONLY);
        if(fd < 0){
        	condlog(0, "%s: Unable to open device ", dev);
		return MPATH_PR_FILE_ERROR;
         }

	if (mpath_mx_alloc_len)
		mx_resp_len = mpath_mx_alloc_len;
	else
		mx_resp_len = get_prin_length(rq_servact);

	if (mx_resp_len == 0) {
		status = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}

	cdb[1] = (unsigned char)(rq_servact & 0x1f);
	cdb[7] = (unsigned char)((mx_resp_len >> 8) & 0xff);
	cdb[8] = (unsigned char)(mx_resp_len & 0xff);

retry :
	memset(&Sensedata, 0, sizeof(SenseData_t));
	memset(&io_hdr,0 , sizeof( struct sg_io_hdr));

	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = MPATH_PRIN_CMDLEN;
	io_hdr.mx_sb_len = sizeof (SenseData_t);
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.cmdp = cdb;
	io_hdr.sbp = (void *)&Sensedata;
	io_hdr.timeout = TIMEOUT;



	io_hdr.dxfer_len = mx_resp_len;
	io_hdr.dxferp = (void *)resp;

	ret =ioctl(fd, SG_IO, &io_hdr);
	if (ret < 0){
		condlog(0, "%s: IOCTL failed %d", dev, ret);
		status = MPATH_PR_OTHER;
		goto out;
	}

	got = mx_resp_len - io_hdr.resid;

	condlog(2, "%s: duration = %u (ms)", dev, io_hdr.duration);
	condlog(2, "%s: persistent reservation in: requested %d bytes but got %d bytes)", dev, mx_resp_len, got);

	status = mpath_translate_response(dev, io_hdr, Sensedata, noisy);

	if (status == MPATH_PR_SENSE_UNIT_ATTENTION && (retry > 0))
	{
		--retry;
		condlog(2, "%s: retrying for Unit Attention. Remaining retries = %d", dev, retry);
		goto retry;
	}

	if (((status == MPATH_PR_SENSE_NOT_READY )&& (Sensedata.ASC == 0x04)&&
				(Sensedata.ASCQ == 0x07))&& (retry > 0))
	{
		usleep(1000);
		--retry;
		condlog(2, "%s: retrying for 02/04/07. Remaining retries = %d", dev, retry);
		goto retry;
	}

	if (status != MPATH_PR_SUCCESS)
		goto out;

	if (noisy)
		dumpHex((const char *)resp, got , 1);


	switch (rq_servact)
	{
		case MPATH_PRIN_RKEY_SA :
			mpath_format_readkeys(resp, got, noisy);
			break;
		case MPATH_PRIN_RRES_SA :
			mpath_format_readresv(resp, got, noisy);
			break;
		case MPATH_PRIN_RCAP_SA :
			mpath_format_reportcapabilities(resp, got, noisy);
			break;
		case MPATH_PRIN_RFSTAT_SA :
			mpath_format_readfullstatus(resp, got, noisy);
	}

out:
	close(fd);
	return status;
}
