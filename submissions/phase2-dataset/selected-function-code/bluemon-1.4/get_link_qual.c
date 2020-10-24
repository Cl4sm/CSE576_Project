{
	struct hci_conn_info_req *cr;
	struct hci_request rq;
#ifdef OCF_GET_LINK_QUALITY
	get_link_quality_rp rp;
#else
   read_link_quality_rp rp;
#endif
   uint16_t handle;
   int qual;

   // check the quality
   if (opts.linkquality || args.linkquality) {
      cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
      if (!cr)
         return;

      bacpy(&cr->bdaddr, &(btdev->bdaddr));
      cr->type = ACL_LINK;
      if (ioctl(btdev->dd, HCIGETCONNINFO, (unsigned long) cr) < 0) {
#ifdef DEBUG
         message(true, "Get connection info failed, trying reconnect\n");
#endif
         return -1;
      }

      handle = htobs(cr->conn_info->handle);

      memset(&rq, 0, sizeof(rq));
      rq.ogf    = OGF_STATUS_PARAM;
#ifdef OCF_GET_LINK_QUALITY
      rq.ocf    = OCF_GET_LINK_QUALITY;
#else
      rq.ocf    = OCF_READ_LINK_QUALITY;
#endif
      rq.cparam = &handle;
      rq.clen   = 2;
      rq.rparam = &rp;
#ifdef OCF_GET_LINK_QUALITY_RP_SIZE
      rq.rlen   = GET_LINK_QUALITY_RP_SIZE;
#else
      rq.rlen   = READ_LINK_QUALITY_RP_SIZE;
#endif

      if (hci_send_req(btdev->dd, &rq, 100) < 0) {
         message(true, "HCI get_link_quality request failed, trying reconnect\n");
         free(cr);
         return -1;
      }

      if (rp.status) {
#ifdef DEBUG
         message(true, "HCI get_link_quality cmd failed (0x%2.2X), trying reconnect\n", rp.status);
#endif
         free(cr);
         return -1;
      }

      qual = rp.link_quality;
      free(cr);
      return qual;
   } 
   // just check for connection
   else {
      qual = hci_for_each_dev(HCI_UP, find_conn, (long) &(btdev->bdaddr));
      return qual < 0 ? qual : 255;
   }
}
