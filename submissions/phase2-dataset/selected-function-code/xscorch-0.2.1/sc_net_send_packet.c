/* sc_net_send_packet()
   Try to queue a packet; true indicates success. */

   dword *header;

   if(!conn || !packet || !packet->data) return(false);

   /* Set up the header dwords on the end of the data. */
   header = (dword *)(packet->data + packet->data_size);
   *header++ = htonl(packet->next_rnd);
   *header++ = htonl(packet->msg_type);
   *header++ = htonl(packet->data_size);

   /* Queue the packet data. */
   if(tn_write(conn->connection, packet->data, packet->data_size + SC_PACKET_HEADER_SIZE))
      return(true);
   else
      return(false);

}
