static void create_plugin(GtkWidget *vbox, gint first_create)
{
  if(first_create) {
    chart = gkrellm_chart_new0();
  }
  
  gkrellm_set_chart_height_default(chart, PLUGIN_HEIGHT);
  gkrellm_chart_create(vbox, mon, chart, &chart_config);
  
  if (first_create) {
    hdplop_main(chart->w, chart->h, chart->drawing_area->window);
  } else {
    dockimlib2_gkrellm_xinit(app->dock, chart->drawing_area->window);
    reshape(chart->w, chart->h);
  }
  if (first_create) {
    //printf("chart : w=%d, h=%d\n", chart->w, chart->h);
    gtk_signal_connect(GTK_OBJECT(chart->drawing_area),
		       "expose_event", (GtkSignalFunc) chart_expose_event, NULL); 
    gtk_signal_connect(GTK_OBJECT(chart->drawing_area),
		       "button_release_event", GTK_SIGNAL_FUNC(button_release_event), NULL);
    g_signal_connect(G_OBJECT(chart->drawing_area),
		       "scroll_event", G_CALLBACK(wheel_event), NULL);
  }
  /* Update plugin every TIMER1 ms */
  if (!timeout_id)
    timeout_id = g_timeout_add(TIMER1, (GtkFunction) update_plugin, NULL);
  gkrellm_disable_plugin_connect(mon, disable_plugin);
}