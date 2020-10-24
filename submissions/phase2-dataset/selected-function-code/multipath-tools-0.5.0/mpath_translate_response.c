{
	condlog(3, "%s: status driver:%02x host:%02x scsi:%02x", dev, 
			io_hdr.driver_status, io_hdr.host_status ,io_hdr.status);
	io_hdr.status &= 0x7e;
	if ((0 == io_hdr.status) && (0 == io_hdr.host_status) &&
			(0 == io_hdr.driver_status))
	{
		return MPATH_PR_SUCCESS;
	}

	switch(io_hdr.status)
	{
		case SAM_STAT_GOOD:
			break;
		case SAM_STAT_CHECK_CONDITION:
			condlog(2, "%s: Sense_Key=%02x, ASC=%02x ASCQ=%02x", dev,
					Sensedata.Sense_Key, Sensedata.ASC, Sensedata.ASCQ);
			switch(Sensedata.Sense_Key)
			{
				case NO_SENSE:
					return MPATH_PR_NO_SENSE;
				case RECOVERED_ERROR:
					return MPATH_PR_SUCCESS;
				case NOT_READY:
					return MPATH_PR_SENSE_NOT_READY;
				case MEDIUM_ERROR:
					return MPATH_PR_SENSE_MEDIUM_ERROR;
				case BLANK_CHECK:
					return MPATH_PR_OTHER;
				case HARDWARE_ERROR:
					return MPATH_PR_SENSE_HARDWARE_ERROR;
				case ILLEGAL_REQUEST:
					return MPATH_PR_ILLEGAL_REQ;
				case UNIT_ATTENTION:
					return MPATH_PR_SENSE_UNIT_ATTENTION;
				case DATA_PROTECT:
				case COPY_ABORTED:
					return MPATH_PR_OTHER;
				case ABORTED_COMMAND:
					return MPATH_PR_SENSE_ABORTED_COMMAND;

				default :
					return MPATH_PR_OTHER;
			}
		case SAM_STAT_RESERVATION_CONFLICT:
			return MPATH_PR_RESERV_CONFLICT;

		default :
			return  MPATH_PR_OTHER;
	}

	switch(io_hdr.host_status)
	{
		case DID_OK :
			break;
		default :
			return MPATH_PR_OTHER;
	}
	switch(io_hdr.driver_status)
	{
		case DRIVER_OK:
			break;
		default :
			return MPATH_PR_OTHER;
	}
	return MPATH_PR_SUCCESS;
}
