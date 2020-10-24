static void
mcd_slacker_class_init (McdSlackerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->constructor = mcd_slacker_constructor;
  object_class->constructed = mcd_slacker_constructed;
  object_class->dispose = mcd_slacker_dispose;

  g_type_class_add_private (klass, sizeof (McdSlackerPrivate));

  /**
   * McdSlacker::inactivity-changed:
   * @self: what a slacker
   * @inactive: %TRUE if the device is inactive.
   *
   * The ::inactivity-changed is emitted when session becomes idle.
   */
  signals[SIG_INACTIVITY_CHANGED] = g_signal_new ("inactivity-changed",
      MCD_TYPE_SLACKER, G_SIGNAL_RUN_LAST, 0, NULL, NULL,
      NULL, G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
}