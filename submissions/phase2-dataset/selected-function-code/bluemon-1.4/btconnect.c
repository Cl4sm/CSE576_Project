bool btconnect(btdev_t* btdev)
{
	int ptype;
	uint16_t handle = 0;
	uint8_t role;
   bool rval;
   struct hci_conn_info_req *cr;
   struct hci_request rq;
   auth_requested_cp acp;
   evt_auth_complete arp;
   evt_encrypt_change erp;
   set_conn_encrypt_cp ecp;

	role = 0x01;
	ptype = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;

   int dev_id = hci_for_each_dev(HCI_UP, find_conn, (long) &(btdev->bdaddr));
	if (dev_id < 0) {
      message(true, "Connecting to: %x:%x:%x:%x:%x:%x\n", btdev->bdaddr.b[5], btdev->bdaddr.b[4], btdev->bdaddr.b[3], btdev->bdaddr.b[2], btdev->bdaddr.b[1], btdev->bdaddr.b[0]);
      if (hci_create_connection(btdev->dd, &(btdev->bdaddr), htobs(ptype), 0, role, &handle, BTCONNECTTIMEOUT) < 0) { 
         message(true, "Can't create connection\n");
         return false;
      }
      btdev->connected = true;
   }
   else btdev->connected = false;

   if (0 == handle) {
      cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));

      bacpy(&cr->bdaddr, &(btdev->bdaddr));
      cr->type = ACL_LINK;
      if (ioctl(btdev->dd, HCIGETCONNINFO, (unsigned long) cr) < 0) {
         message(true, "Get connection info failed");
         return false;
      }
      handle = htobs(cr->conn_info->handle);
      free(cr);
   }
   
   if (opts.authenticate || args.authenticate) {
   
      acp.handle = htobs(handle);
      
      memset(&rq, 0, sizeof(rq));
      rq.ogf    = OGF_LINK_CTL;
      rq.ocf    = OCF_AUTH_REQUESTED;
      rq.cparam = &acp;
      rq.clen   = AUTH_REQUESTED_CP_SIZE;
      rq.rparam = &arp;
      rq.rlen   = EVT_AUTH_COMPLETE_SIZE;
      rq.event  = EVT_AUTH_COMPLETE;

      if (hci_send_req(btdev->dd, &rq, 25000) < 0) {
#ifdef DEBUG
         message(true, "Can't authenticate link\n");
#endif
         return false;
      }
      ecp.handle = htobs(handle);
      ecp.encrypt = 1;

      memset(&rq, 0, sizeof(rq));
      rq.ogf    = OGF_LINK_CTL;
      rq.ocf    = OCF_SET_CONN_ENCRYPT;
      rq.cparam = &ecp;
      rq.clen   = SET_CONN_ENCRYPT_CP_SIZE;
      rq.rparam = &erp;
      rq.rlen   = EVT_ENCRYPT_CHANGE_SIZE;
      rq.event  = EVT_ENCRYPT_CHANGE;

      if (hci_send_req(btdev->dd, &rq, 25000) < 0) {
#ifdef DEBUG
         message(true, "Can't encrypt link\n");
#endif
         return false;
      }
   }
   return true;
}
