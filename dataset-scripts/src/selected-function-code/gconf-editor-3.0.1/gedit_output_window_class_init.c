static void
gedit_output_window_class_init (GeditOutputWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

   	object_class->finalize = gedit_output_window_finalize;

	signals[CLOSE_REQUESTED] = 
		g_signal_new ("close_requested",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GeditOutputWindowClass, close_requested),
			      NULL, 
			      NULL,
			      g_cclosure_marshal_VOID__VOID,
			      G_TYPE_NONE, 
			      0);
	signals[SELECTION_CHANGED] = 
		g_signal_new ("selection_changed",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GeditOutputWindowClass, selection_changed),
			      NULL, 
			      NULL,
			      g_cclosure_marshal_VOID__STRING,
			      G_TYPE_NONE, 
			      1, G_TYPE_STRING);

}