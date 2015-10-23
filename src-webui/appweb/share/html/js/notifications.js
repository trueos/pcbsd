var notices;
var nstatus;
var wstatus;

function init_ws_notification()
{
  notices = document.getElementById("notifier");
  connectWebSocket();
}

function connectWebSocket()
{
  websocket = new WebSocket(wsserver);
  websocket.onopen = function(evt) { onOpen(evt) };
  websocket.onclose = function(evt) { onClose(evt) };
  websocket.onmessage = function(evt) { onMessage(evt) };
  websocket.onerror = function(evt) { onError(evt) };
}

function onOpen(evt)
{
  var authjson = '{ "namespace":"rpc", "name":"auth_token", "id":"authrequest", "args": { "token":"' + wstoken + '" } }';
  nstatus = "auth";
  wstatus = "idle";
  doSend(authjson);
}

function onClose(evt)
{
}

function onMessage(evt)
{
  // We are in the auth phase
  if ( nstatus == "auth" ) {
    var jsonobj = JSON.parse(evt.data);
    if ( jsonobj.code == "401" ) {
      replaceContents('<p>FAILED AUTH</p>');
    } else {
      nstatus = "events";
      var eventjson = '{ "namespace":"events", "name":"subscribe", "id":"subrequest", "args":["dispatcher"] }';
      doSend(eventjson);
      return;
    }
  }

  // Event has arrived
  parseNoticeEvents(evt.data);
}

// Lets parse incoming messages from dispatcher
function parseNoticeEvents(incoming)
{
  var jsonobj = JSON.parse(incoming);
  if ( jsonobj['args'].subscribe == "dispatcher" )
    return;

  // Get the URL page variable
  var page = getAppUrlVars()["p"];

  if ( jsonobj['args'].args == "idle" ) {

    // Look for a previous busy indicator
    // If found, refresh the page, since we aren't busy anymore
    if ( document.getElementById('item-working') !== null ) {
      var t = setTimeout('location.reload(true)', 500);
    }

    replaceContents('<a href=\"?p=dispatcher&ref=' + page + '\"><img align=absmiddle height=32 width=32 src=\"../images/dialog-ok.png\"> Status</a>');
    if ( wstatus == "working" )
      var t = setTimeout('location.reload(true)', 2000);
    wstatus = "idle";
    return;
  }
  //window.alert(incoming);
  replaceContents('<a href=\"?p=dispatcher&ref=' + page + '\"><img align=absmiddle height=32 width=32 src=\"../images/working.gif\" title=\"' + jsonobj['args'].args + '\"> Working</a>');
  wstatus = "working";
}

function onError(evt)
{
  replaceContents('<p><span style="color: red;">ERROR:' + evt.data + '</span></p>');
}

function doSend(message)
{
  //replaceContents("<p>SENT: " + message + "</p>"); 
  websocket.send(message);
}

function replaceContents(message)
{
  notices.innerHTML = message;
}

function getAppUrlVars() {
    var vars = {};
    var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi,    
    function(m,key,value) {
      vars[key] = value;
    });
    return vars;
}

window.addEventListener("load", init_ws_notification, false);
