static void output_png_flush(struct output_data_t *out)
{
	int i;
	struct private_png_t *prv=out->private_format_data;
	prv->rows++;
	prv->data= realloc(prv->data,prv->rows*out->columns*sizeof(float));
	for(i=0;i<out->columns;++i) {
		switch (out->data[i].type) {
			case TYPE_int: 
				prv->data[out->columns*(prv->rows-1)+i]=out->data[i].d.d_int;
				break;
			case TYPE_str:
				free(out->data[i].d.d_str);
				break;
			case TYPE_float:
				prv->data[out->columns*(prv->rows-1)+i]=out->data[i].d.d_float;
				break;
			case TYPE_time:
				prv->data[out->columns*(prv->rows-1)+i]=out->data[i].d.d_time;
				break;
		}
	}
}