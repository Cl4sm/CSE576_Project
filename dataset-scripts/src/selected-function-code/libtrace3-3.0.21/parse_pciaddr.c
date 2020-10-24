static int parse_pciaddr(char * str, struct rte_pci_addr * addr, long * core) {
    char * wrkstr;
    char * pch;
    assert(str);
    wrkstr = strdup(str);
    
    pch = strtok(wrkstr,":");
    if (pch == NULL || pch[0] == 0) {
        free(wrkstr); return -1;
    }
    addr->domain = (uint16_t) atoi(pch);

    pch = strtok(NULL,":");
    if (pch == NULL || pch[0] == 0) {
        free(wrkstr); return -1;
    }
    addr->bus = (uint8_t) atoi(pch);

    pch = strtok(NULL,".");
    if (pch == NULL || pch[0] == 0) {
        free(wrkstr); return -1;
    }
    addr->devid = (uint8_t) atoi(pch);

    pch = strtok(NULL,"-"); /* Might not find the '-' it's optional */
    if (pch == NULL || pch[0] == 0) {
        free(wrkstr); return -1;
    }
    addr->function = (uint8_t) atoi(pch);

    pch = strtok(NULL, ""); /* Find end of string */
    
    if (pch != NULL && pch[0] != 0) {
        *core = (long) atoi(pch);
    }

    free(wrkstr);
    return 0;
}