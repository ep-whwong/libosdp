Control Panel
=============

The following functions are used when OSDP is to be used in CP mode. The library
returns a single opaque pointer of type ``osdp_t`` where it maintains all it's
internal data. All applications consuming this library must pass this context
pointer all API calls.

For the CP application, it's connected PDs are referenced by the offset number
(0-indexed). This offset corresponds to the order in which the
``osdp_pd_info_t`` was populated when passed to ``osdp_cp_setup``.

Device lifecycle management
---------------------------

.. doxygentypedef:: osdp_t

.. doxygenfunction:: osdp_cp_setup

.. doxygenfunction:: osdp_cp_refresh

.. doxygenfunction:: osdp_cp_teardown

Events
------

Events are generated by the PD and sent to the CP. The CP app can register a
callback using ``osdp_cp_set_event_callback`` to get notified of events.

.. doxygentypedef:: cp_event_callback_t

.. doxygenfunction:: osdp_cp_set_event_callback

Refer to the `event structure`_ document for more information on how the
``event`` structure is framed.

.. _event structure: event-structure.html

Commands
--------

Commands are sent from the CP to the PD to perform various actions. The CP app
has to create a command struct and then call ``osdp_cp_submit_command`` to enqueue
the command to a particular PD.

.. doxygenfunction:: osdp_cp_submit_command

.. doxygenfunction:: osdp_cp_flush_commands

Refer to the `command structure`_ document for more information on how to
populate the ``cmd`` structure for these function.

.. _command structure: command-structure.html

Get PD capability
-----------------

.. doxygenstruct:: osdp_pd_cap
   :members:

.. doxygenfunction:: osdp_cp_get_capability

Others
------

.. doxygenfunction:: osdp_cp_get_pd_id

.. doxygenfunction:: osdp_cp_modify_flag

