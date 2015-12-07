==========
Connection
==========

Some intro text here...

Getting Started
---------------

Add some links to docs on websockets and json....

Authentication
--------------

Describe how to authenticate to websockets via Local / Remote, local connections do not need username / password...



Once a websocket connection is made to the server, the client needs to authenticate itself to obtain access to the syscache service. Several methods are available for authentication:

**JSON Request: User/Password Login**

.. code-block:: json

  {
  "namespace" : "rpc",
  "name" : "auth",
  "id" : "sampleID",
  "args" : { 
          "username" : "myuser", 
          "password" : "mypassword" 
          }
  }

**JSON Request: Re-saved Token Authentication (token is invalidated after 5 minutes of inactivity)**

.. code-block:: json

  {
  "namespace" : "rpc",
  "name" : "auth_token",
  "id" : "sampleID",
  "args" : { 
          "token" : "MySavedAuthToken"
          }
  }

**JSON Reply: Valid Authentication**

.. code-block:: json

  {
    "args": [
        "SampleAuthenticationToken",
        300
    ],
    "id": "sampleID",
    "name": "response",
    "namespace": "rpc"
  }

.. _note: the first element of the *"args"* array is the authentication token for use later as necessary, while the second element is the number of seconds for which that token is valid.
   The token is reset after every successful communication with the websocket. In this example, it is set to 5 minutes of inactivity before the token is invalidated. The websocket server is
   currently set to close any connection to a client after 10 minutes of inactivity.

**JSON Reply: Invalid Authentication (this may also happen for any type of system request if the user session timed out due to inactivity)**

.. code-block:: json

  {
    "args": {
        "code": 401,
        "message": "Unauthorized"
    },
    "id": "sampleID",
    "name": "error",
    "namespace": "rpc"
  }

**JSON Request: Clear Pre-saved Authentication Token (such as signing out)**

.. code-block:: json

  {
  "namespace" : "rpc",
  "name" : "auth_clear",
  "id" : "sampleID",
  "args" : "junk argument"
  }
