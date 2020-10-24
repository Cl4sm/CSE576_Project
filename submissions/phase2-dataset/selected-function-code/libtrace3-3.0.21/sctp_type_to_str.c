static char *sctp_type_to_str(uint8_t type)
{
    switch(type)
    {
        case 0: return "DATA";
        case 1: return "INIT";
        case 2: return "INIT ACK";
        case 3: return "SACK";
        case 4: return "HEARTBEAT";
        case 5: return "HEARTBEAT ACK";
        case 6: return "ABORT";
        case 7: return "SHUTDOWN";
        case 8: return "SHUTDOWN ACK";
        case 9: return "ERROR";
        case 10: return "COOKIE ECHO";
        case 11: return "COOKIE ACK";
        case 12: return "Reserved for ECNE";
        case 13: return "Reserved for CWR";
        case 14: return "SHUTDOWN COMPLETE";
        case 63:
        case 127:
        case 191:
        case 255: return "IETF-defined Chunk Extensions";
    };

   return "reserved by IETF";
}