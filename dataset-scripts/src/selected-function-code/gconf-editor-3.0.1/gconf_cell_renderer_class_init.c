static void
gconf_cell_renderer_class_init (GConfCellRendererClass *klass)
{
	GObjectClass *object_class = (GObjectClass *)klass;
	GtkCellRendererClass *cell_renderer_class = (GtkCellRendererClass *)klass;

	object_class->get_property = gconf_cell_renderer_get_property;
	object_class->set_property = gconf_cell_renderer_set_property;
	object_class->finalize = gconf_cell_renderer_finalize;

	cell_renderer_class->get_size = gconf_cell_renderer_get_size;
	cell_renderer_class->render = gconf_cell_renderer_render;
	cell_renderer_class->activate = gconf_cell_renderer_activate;
	cell_renderer_class->start_editing = gconf_cell_renderer_start_editing;

	g_object_class_install_property (object_class, PROP_VALUE,
					 g_param_spec_boxed ("value",
							     NULL, NULL,
							     GCONF_TYPE_VALUE,
							     G_PARAM_READWRITE));

	gconf_cell_signals[CHANGED] =
		g_signal_new ("changed",
			      G_TYPE_FROM_CLASS (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GConfCellRendererClass, changed),
			      (GSignalAccumulator) NULL, NULL,
			      gconf_marshal_VOID__STRING_BOXED,
			      G_TYPE_NONE, 2,
			      G_TYPE_STRING,
			      GCONF_TYPE_VALUE);
	gconf_cell_signals[CHECK_WRITABLE] =
		g_signal_new ("check_writable",
			      G_TYPE_FROM_CLASS (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GConfCellRendererClass, changed),
			      (GSignalAccumulator) NULL, NULL,
			      gconf_marshal_BOOLEAN__STRING,
			      G_TYPE_BOOLEAN, 1,
			      G_TYPE_STRING);
}