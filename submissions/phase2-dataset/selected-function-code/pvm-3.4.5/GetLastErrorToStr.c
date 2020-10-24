char *
GetLastErrorToStr (error)
int error;
{
    switch(error)
    {
        case WSAENETDOWN:
            return("The network subsystem has failed.\n");
            break;
        case WSAEINTR:
            return("A blocking call was cancelled.  This can be caused \
		by\n1) a short response time, or\n2) \
		User interrupts the process.\n");
            break;
        case WSAEINPROGRESS:
            return("A blocking call is in progress.\n");
            break;
        case WSAENOBUFS:
            return("No buffer space is available.\n");
            break;
        case WSAENOTSOCK:
            return("Invalid socket descriptor.\n");
            break;
        case WSAEADDRINUSE:
            return("The specified address is already in use.\n");
            break;
        case WSAEADDRNOTAVAIL:
            return("The specified address is not available\nfrom the local machine.\n");
            break;
        case WSAECONNREFUSED:
            return("The connection attempt was refused.\n");
            break;
        case WSAEINVAL:
            return("The socket is not bound to an address.\n");
            break;
        case WSAEISCONN:
            return("The socket is already connected.\n");
            break;
        case WSAEMFILE:
            return("The maximum number of sockets has exceeded.\n");
            break;
        case WSAENETUNREACH:
            return("Network cannot be reached from this host at this time.\n");
            break;
        case WSAETIMEDOUT:
            return("Attempt to connect timed out without establishing a connection.\n");
            break;
        case WSAENOTCONN:
            return("The socket is not connected.\n");
            break;
        case WSAESHUTDOWN:
            return("The socket has been shut down.\n");
            break;
        case WSAECONNABORTED:
            return("The socket connection has been reset.\n");
            break;
        case WSAECONNRESET:
            return("Socket shutdown by remote.\n");
            break;
        case WSAEACCES:
            return("The requested address is a broadcast address.\n");
            break;
        case WSAENETRESET:
            return("The socket connection has been reset.\n");
            break;
        case WSAHOST_NOT_FOUND:
            return("Host could not found.\n");
            break;
        default:
	    return("could not match error tag");
            break;
    }
}
