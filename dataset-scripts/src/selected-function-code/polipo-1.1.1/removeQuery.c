static void
removeQuery(DnsQueryPtr query)
{
    DnsQueryPtr previous;
    if(query == inFlightDnsQueries) {
        inFlightDnsQueries = query->next;
        if(inFlightDnsQueries == NULL)
            inFlightDnsQueriesLast = NULL;
    } else {
        previous = inFlightDnsQueries;
        while(previous->next) {
            if(previous->next == query)
                break;
            previous = previous->next;
        }
        assert(previous->next != NULL);
        previous->next = query->next;
        if(previous->next == NULL)
            inFlightDnsQueriesLast = previous;
    }
}