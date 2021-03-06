.. -*- rst -*-

====================================================
 Create external events (os-server-external-events)
====================================================

.. warning::
   This is an ``admin`` level service API only designed to be used by
   other OpenStack services. The point of this API is to coordinate
   between Nova and Neutron (and potentially future services) on
   activities they both need to be involved in, such as network
   hotplugging.

   Unless you are writing Neutron code you **should not** be using
   this API.

Creates one or more external events. The API dispatches each event to a
server instance.

Run Events
==========

.. rest_method:: POST /v2.1/{tenant_id}/os-server-external-events

Creates one or more external events, which the API dispatches to the
host a server is assigned to. If the server is not currently assigned
to a host the event will not be delivered.

You will receive back the list of events that you submitted, with an
updated ``code`` and ``status`` indicating their level of success.

Normal response codes: 200, 207

A 200 will be returned if all events succeeded, 207 will be returned
if some events could not be processed. The ``code`` attribute for the
event will explain further what went wrong.

Error response codes: badRequest(400), unauthorized(401), forbidden(403),
itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - events: events
  - name: event_name
  - server_uuid: server_uuid
  - status: event_status
  - tag: event_tag

**Example Run Events**

.. literalinclude:: ../../doc/api_samples/os-server-external-events/event-create-req.json
   :language: javascript

Response
--------

.. rest_parameters:: parameters.yaml

  - events: events
  - code: code
  - name: event_name
  - server_uuid: server_uuid
  - status: event_status
  - tag: event_tag

**Example Run Events**

.. literalinclude:: ../../doc/api_samples/os-server-external-events/event-create-resp.json
   :language: javascript
