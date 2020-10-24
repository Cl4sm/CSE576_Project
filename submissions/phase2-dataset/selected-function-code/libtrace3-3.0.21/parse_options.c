static void parse_options(char *offset, int vlen)
{
    while(vlen > 0) {
        struct sctp_var_param_hdr *ph = (struct sctp_var_param_hdr *)(offset);
        char *data = (char *)(ph + 1);

        switch(ntohs(ph->type)) {
            case 5:
            {
                struct in_addr *ia = (struct in_addr *)data;
                printf(" SCTP: Option IP address %s\n", inet_ntoa(*ia));
            }
            break;
            case 6:
            {
                printf(" SCTP: Option IPv6 address (TODO)\n");
            }
            break;
            case 7:
            {
                printf(" SCTP: Option State cookie\n");
                /* // Prolly don't want to print this out :)
                for(int i = 0; i < ntohs(ph->length) - 8; i++)
                    printf("%02x", data[i]);
                printf("'\n");*/
            }
            break;
            case 9:
            {
                printf(" SCTP: Option Cookie preservative (TODO)\n");
            }
            break;
            case 11:
            {
                printf(" SCTP: Option Host name %s\n", data);
            }
            break;
            case 12:
            {
                uint16_t *p = (uint16_t *)data;
                int len = ntohs(ph->length) - 
                    sizeof(struct sctp_var_param_hdr);
                
                printf(" SCTP: Option Supported address types ");
                
                while(len) {
                    printf("%hu ", ntohs(*p));
                    p++;
                    len -= sizeof(*p);
                }
                printf("\n");
            }
            break;
            default:
                printf(" SCTP: Option Unknown type=%hu len=%hu\n", 
                        ntohs(ph->type), ntohs(ph->length));
        }

        vlen -= ntohs(ph->length);
        offset += ntohs(ph->length);
    }
}