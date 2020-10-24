/* sc_net_recv_packet()
   Pull packets off the network stream and tokenize them into the packet queue.
   Then return a single packet off the top of the queue. */

   sc_packet packet;
   dword *header;
   size_t size;

   /* Scan for new packets. */
   tn_scan_read(conn->connection, NULL);

   if(tn_read(conn->connection, &packet.data, &size)) {
      if(packet.data == NULL || size < SC_PACKET_HEADER_SIZE) {
         /* Supersmall packets! (This is by definition not possible!) */
         sc_net_set_error("recv_packet", "Packet size fields mismatch");
         SC_CONN_SET_FLAGS(*conn, SC_CONN_TCPNET_ERROR);
         sc_net_packet_release(&packet);
         return(false);
      }

      /* We read packet data.  Munch it! */
      header = (dword *)(packet.data + size);
      packet.data_size = ntohl(*(--header));
      packet.msg_type  = ntohl(*(--header));
      packet.next_rnd  = ntohl(*(--header));

      /* This is a little helper to catch buffer overwrites. */
      strcopyb((char *)header, "BUFOFLO", 2 * sizeof(dword));

      if(size != packet.data_size + SC_PACKET_HEADER_SIZE) {
         /* A badly damaged packet came in (shouldn't happen). */
         sc_net_set_error("recv_packet", "Packet size fields mismatch");
      }

      /* Handle the packet. */
      handler(c, parm, &packet);

      /* Clean up. */
      sc_net_packet_release(&packet);

      /* Ask for a rescan in case there are more packets. */
      return(true);
   } else {
      /* Check for error conditions. */
      if(TN_STATE_IS_ERROR(conn->connection)) {
         sc_net_set_error("recv_packet", tn_error_string(conn->connection));
         if(!TN_CONNECTION_IS_UP(conn->connection))
            /* A fatal error killed the connection. */
            SC_CONN_SET_FLAGS(*conn, SC_CONN_LOCAL_ERROR);
         else
            /* Eventually, the TCP NET library will have error recovery. */
            SC_CONN_SET_FLAGS(*conn, SC_CONN_TCPNET_ERROR);
      }
      return(false);
   }

}
