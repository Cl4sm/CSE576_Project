static void output_html_flush(struct output_data_t *out)
{
	int i;
	printf(" <tr class=\"%s\">\n",((*(int*)out->private_format_data)++)&1?"odd":"even");
	for(i=0;i<out->columns;++i) {
		switch (out->data[i].type) {
			case TYPE_int: 
				printf("  <td class=\"numeric\">%" PRIu64 "</td>\n",out->data[i].d.d_int);
				break;
			case TYPE_str:
				printf("  <td>%s</td>\n",out->data[i].d.d_str);
				free(out->data[i].d.d_str);
				break;
			case TYPE_float:
				printf("  <td class=\"numeric\">%f</td>\n",out->data[i].d.d_float);
				break;
			case TYPE_time:
				printf("  <td class=\"numeric\">%.03f</td>\n",out->data[i].d.d_time);
				break;
		}
	}
	printf(" </tr>\n");
}