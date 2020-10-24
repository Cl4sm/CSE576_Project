/* sc_accessory_inventory_clear
   Clear out the player accessory inventories. */

   sc_registry_iter *iter;

   if(ac == NULL) return;

   /* Prepare a registry iterator. */
   iter = sc_registry_iter_new(ac->registry, ac->registryclass, SC_REGISTRY_FORWARD,
                               _sc_accessory_inventory_clear_func, 0);

   /* Iterate the entire accessories registry,
      with the side effect of erasing inventories. */
   sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

}
