*checker_msg_string(struct volume_access_inq *inq)
{
	/* lun not connected */
	if (((inq->PQ_PDT & 0xE0) == 0x20) || (inq->PQ_PDT & 0x7f))
		return MSG_RDAC_DOWN_TYPE("lun not connected");

	/* if no tpg data is available, give the generic path down message */
	if (!(inq->avtcvp & 0x10))
		return MSG_RDAC_DOWN;

	/* controller is booting up */
	if (((inq->aas_cur & 0x0F) == RTPG_TRANSITIONING) &&
		(inq->aas_alt & 0x0F) != RTPG_TRANSITIONING)
		return MSG_RDAC_DOWN_TYPE("ctlr is in startup sequence");

	/* if not unavailable, give generic message */
	if ((inq->aas_cur & 0x0F) != RTPG_UNAVAILABLE)
		return MSG_RDAC_DOWN;

	/* target port group unavailable */
	switch (inq->vendor_specific_cur) {
	case RTPG_UNAVAIL_NON_RESPONSIVE:
		return MSG_RDAC_DOWN_TYPE("non-responsive to queries");
	case RTPG_UNAVAIL_IN_RESET:
		return MSG_RDAC_DOWN_TYPE("ctlr held in reset");
	case RTPG_UNAVAIL_CFW_DL1:
	case RTPG_UNAVAIL_CFW_DL2:
		return MSG_RDAC_DOWN_TYPE("ctlr firmware downloading");
	case RTPG_UNAVAIL_QUIESCED:
		return MSG_RDAC_DOWN_TYPE("ctlr quiesced by admin request");
	case RTPG_UNAVAIL_SERVICE_MODE:
		return MSG_RDAC_DOWN_TYPE("ctlr is in service mode");
	default:
		return MSG_RDAC_DOWN_TYPE("ctlr is unavailable");
	}
}
