static void output_txt_flush(struct output_data_t *out)
{
	int i;
	for(i=0;i<out->columns;++i) {
		switch (out->data[i].type) {
			case TYPE_int: 
				printf("%*" PRIu64 " ",COLWIDTH-1,out->data[i].d.d_int);
				break;
			case TYPE_str:
				printf("%*s ",COLWIDTH-1,out->data[i].d.d_str);
				free(out->data[i].d.d_str);
				break;
			case TYPE_float:
				printf("%*f ",COLWIDTH-1,out->data[i].d.d_float);
				break;
			case TYPE_time:
				printf("%*.0f ",COLWIDTH-1,out->data[i].d.d_time);
				break;
		}
	}
	printf("\n");
}