.. -*- rst -*-
.. needs:parameter_verification
.. needs:example_verification
.. needs:body_verification

=======================================
 Project networks (os-tenant-networks)
=======================================

Creates, lists, shows information for, and deletes project networks.

List Project Networks
=====================

.. rest_method:: GET /v2.1/{tenant_id}/os-tenant-networks

Lists all project networks.

Policy defaults enable only users with the administrative role or
the owner of the network to perform this operation. Cloud providers
can change these permissions through the ``policy.json`` file.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id

Response
--------

**Example List Project Networks: JSON response**

.. literalinclude:: ../../doc/api_samples/os-tenant-networks/networks-list-res.json
   :language: javascript

Create Project Network
======================

.. rest_method:: POST /v2.1/{tenant_id}/os-tenant-networks

Creates a project network.

Policy defaults enable only users with the administrative role to
perform this operation. Cloud providers can change these permissions
through the ``policy.json`` file.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401), forbidden(403), conflict(409), serviceUnavailable(503)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id

**Example Create Project Network: JSON request**

.. literalinclude:: ../../doc/api_samples/os-tenant-networks/networks-post-req.json
   :language: javascript

Response
--------

**Example Create Project Network: JSON response**

.. literalinclude:: ../../doc/api_samples/os-tenant-networks/networks-post-res.json
   :language: javascript

Show Project Network Details
============================

.. rest_method:: GET /v2.1/{tenant_id}/os-tenant-networks/{network_id}

Shows details for a project network.

Policy defaults enable only users with the administrative role or
the owner of the network to perform this operation. Cloud providers
can change these permissions through the ``policy.json`` file.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - network_id: network_id

Response
--------

**Example Show Project Network Details: JSON response**

.. literalinclude:: ../../doc/api_samples/os-tenant-networks/networks-post-res.json
   :language: javascript

Delete Project Network
======================

.. rest_method:: DELETE /v2.1/{tenant_id}/os-tenant-networks/{network_id}

Deletes a project network.

Policy defaults enable only users with the administrative role or
the owner of the network to perform this operation. Cloud providers
can change these permissions through the ``policy.json`` file.

Normal response codes: 202

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404), conflict(409)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - network_id: network_id

Response
--------

There is no body content for the response of a successful DELETE query.
