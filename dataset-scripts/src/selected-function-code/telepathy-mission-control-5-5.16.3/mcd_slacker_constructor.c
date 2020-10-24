static GObject *
mcd_slacker_constructor (
    GType type,
    guint n_construct_properties,
    GObjectConstructParam *construct_properties)
{
  GObject *retval;

  if (slacker == NULL)
    {
      slacker = G_OBJECT_CLASS (mcd_slacker_parent_class)->constructor (
        type, n_construct_properties, construct_properties);
      retval = slacker;
      g_object_add_weak_pointer (retval, &slacker);
    }
  else
    {
      retval = g_object_ref (slacker);
    }

  return retval;
}