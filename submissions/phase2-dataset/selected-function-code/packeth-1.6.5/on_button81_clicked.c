void
on_button81_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget *en1;
	gchar *dst_ip_t;
	int i, j, mc;
	char tmpmac[20], tmp[5];
	
        en1 = lookup_widget(GTK_WIDGET(button), "entry39");
	gtk_entry_set_text(GTK_ENTRY(en1), "94040000");
	
        en1 = lookup_widget(GTK_WIDGET(button), "entry44");
	gtk_entry_set_text(GTK_ENTRY(en1), "1");
	
        en1 = lookup_widget(GTK_WIDGET(button), "entry27");
	gtk_entry_set_text(GTK_ENTRY(en1), "6");
	
        en1 = lookup_widget(GTK_WIDGET(button), "entry37");
	dst_ip_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));

	/* check destination ip address */
        if (check_ip_address(dst_ip_t) == -1) {
                //printf("Error: Wrong destination ipv4 address format\n");
                error("Error: Wrong destination ipv4 address format");
                return;
        }

	memset(tmpmac, 0, 20);
	strcat(tmpmac, "01:00:5E:");

	for (i=0; i<4; i++) {
                for(j=0; j<4 && (*dst_ip_t != '\0'); j++) {
                        if ( ((int)*dst_ip_t == '.') && (i<3) && (j>0) ) {
                                dst_ip_t++;
                                break;
                        }
			tmp[j] = *dst_ip_t;
                        dst_ip_t++;
                }
                tmp[j] = '\0';

		if (i==0) 
			continue;
		else if (i==1) {
                       	mc = atoi(tmp);
			mc = mc & 0x7f;
			c8(tmp, mc);
			strcat(tmpmac, tmp);
			strcat(tmpmac, ":");
		}
		else if (i==2){
                       	mc = atoi(tmp);
			c8(tmp, mc);
			strcat(tmpmac, tmp);
			strcat(tmpmac, ":");
		}
		else {
                       	mc = atoi(tmp);
			c8(tmp, mc);
			strcat(tmpmac, tmp);
		}
        }

        en1 = lookup_widget(GTK_WIDGET(button), "L_dst_mac");
	gtk_entry_set_text(GTK_ENTRY(en1), tmpmac);
}