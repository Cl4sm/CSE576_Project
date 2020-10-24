void
listServers(FILE *out)
{
    HTTPServerPtr server;
    int i, n, m, entry;

    fprintf(out, "<!DOCTYPE HTML PUBLIC "
            "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
            "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
            "<html><head>\n"
            "\r\n<title>Known servers</title>\n"
           "</head><body>\n"
            "<h1>Known servers</h1>\n");

    alternatingHttpStyle(out, "servers");
    fprintf(out, "<table id=servers>\n");
    fprintf(out, "<thead><tr><th>Server</th>"
            "<th>Version</th>"
            "<th>Persistent</th>"
            "<th>Pipeline</th>"
            "<th>Connections</th>"
            "<th></th>"
            "<th>rtt</th>"
            "<th>rate</th>"
            "</tr></thead>\n");
    fprintf(out, "<tbody>\n");
    server = servers;
    entry = 0;
    while(server) {
        fprintf(out, "<tr class=\"%s\">", entry % 2 == 0 ? "even" : "odd");
        if(server->port == 80)
            fprintf(out, "<td>%s</td>", server->name);
        else
            fprintf(out, "<td>%s:%d</td>", server->name, server->port);

        if(server->version == HTTP_11)
            fprintf(out, "<td>1.1</td>");
        else if(server->version == HTTP_10)
            fprintf(out, "<td>1.0</td>");
        else
            fprintf(out, "<td>unknown</td>");

        if(server->persistent < 0)
            fprintf(out, "<td>no</td>");
        else if(server->persistent > 0)
            fprintf(out, "<td>yes</td>");
        else
            fprintf(out, "<td>unknown</td>");

        if(server->version != HTTP_11 || server->persistent <= 0)
            fprintf(out, "<td></td>");
        else if(server->pipeline < 0)
            fprintf(out, "<td>no</td>");
        else if(server->pipeline >= 0 && server->pipeline <= 1)
            fprintf(out, "<td>unknown</td>");
        else if(server->pipeline == 2 || server->pipeline == 3)
            fprintf(out, "<td>probing</td>");
        else 
            fprintf(out, "<td>yes</td>");

        n = 0; m = 0;
        for(i = 0; i < server->maxslots; i++)
            if(server->connection[i] && !server->connection[i]->connecting) {
                if(i < server->numslots)
                    n++;
                else
                    m++;
            }
            
        fprintf(out, "<td>%d/%d", n, server->numslots);
        if(m)
            fprintf(out, " + %d</td>", m);
        else
            fprintf(out, "</td>");

        if(server->lies > 0)
            fprintf(out, "<td>(%d lies)</td>", (server->lies + 9) / 10);
        else
            fprintf(out, "<td></td>");

        if(server->rtt > 0)
            fprintf(out, "<td>%.3f</td>", (double)server->rtt / 1000000.0);
        else
            fprintf(out, "<td></td>");
        if(server->rate > 0)
            fprintf(out, "<td>%d</td>", server->rate);
        else
            fprintf(out, "<td></td>");

        fprintf(out, "</tr>\n");
        server = server->next;
        entry++;
    }
    fprintf(out, "</tbody>\n");
    fprintf(out, "</table>\n");
    fprintf(out, "<p><a href=\"/polipo/\">back</a></p>");
    fprintf(out, "</body></html>\n");
}