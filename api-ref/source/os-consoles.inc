.. -*- rst -*-
.. needs:parameter_verification
.. needs:example_verification
.. needs:body_verification

===============================================================
 Server consoles (servers, os-consoles, os-console-auth-token)
===============================================================

Manages server consoles.

Lists Consoles
==============

.. rest_method:: GET /v2.1/{tenant_id}/servers/{server_id}/consoles

Lists all consoles for a server instance.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - server_id: server_id

Response
--------

**Example List consoles: JSON response**

.. literalinclude:: ../../doc/api_samples/consoles/consoles-list-get-resp.json
   :language: javascript


Create Console
==============

.. rest_method:: POST /v2.1/{tenant_id}/servers/{server_id}/consoles

Creates a console for a server instance.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - server_id: server_id

Response
--------

Show Console Details
====================

.. rest_method:: GET /v2.1/{tenant_id}/servers/{server_id}/consoles/{console_id}

Shows details for a console for a server instance.

Normal response codes: 200

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - server_id: server_id
  - console_id: console_id

Response
--------

**Example List consoles: JSON response**

.. literalinclude:: ../../doc/api_samples/consoles/consoles-get-resp.json
   :language: javascript

Delete Console
==============

.. rest_method:: DELETE /v2.1/{tenant_id}/servers/{server_id}/consoles/{console_id}

Deletes a console for a server instance.

Normal response codes: 202

Error response codes: unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - server_id: server_id
  - console_id: console_id

Response
--------

Show Console Authentication Token
=================================

.. rest_method:: GET /v2.1/{tenant_id}/servers/{server_id}/os-console-auth-token

Shows the authentication token for a console for a server instance.

This feature is available for ``rdp-html5`` console type only.

Normal response codes: 200

Error response codes: badRequest(400), unauthorized(401), forbidden(403), itemNotFound(404)

Request
-------

.. rest_parameters:: parameters.yaml

  - tenant_id: tenant_id
  - server_id: server_id

Response
--------

**Example List consoles: JSON response**

.. literalinclude:: ../../doc/api_samples/os-console-auth-tokens/get-console-connect-info-get-resp.json
   :language: javascript
