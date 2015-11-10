var notices;
var statusbox;
var nstatus;
var wstatus;
var doingstatus = "none";

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

  if ( doingstatus != "none" && jsonobj['id'] == "logrequest" )
  {
    replaceStatusBox(jsonobj['args']["log " + doingstatus]);
    setTimeout(startLogStatus(doingstatus), 1000);
    return;
  }

  if ( jsonobj['args'].subscribe == "dispatcher" )
    return;

  // Get the URL page variable
  var page = getAppUrlVars()["p"];
  var statusline = jsonobj['args'].args;

  if ( statusline == "idle" ) {

    // Look for a previous busy indicator
    // If found, refresh the page, since we aren't busy anymore
    if ( document.getElementById('item-working') !== null && doingstatus == "none" ) {
      var t = setTimeout('location.reload(true)', 500);
    }

    replaceContents('<a href=\"?p=dispatcher&ref=' + page + '\"><img align=absmiddle height=32 width=32 src=\"../images/dialog-ok.png\"> Status</a>');
    if ( wstatus == "working" && doingstatus == "none" )
      var t = setTimeout('location.reload(true)', 2000);
    wstatus = "idle";
    return;
  }
  //window.alert(incoming);
  replaceContents('<a title=\"AppCafe is working. Click for details\" href=\"?p=dispatcher&ref=' + page + '\"><img align=absmiddle height=32 width=32 src=\"../images/working.gif\" title=\"AppCafe is working. Click for details\"> Working</a>');
  wstatus = "working";

  // Check if we are on the status page and can show log output
  if ( (document.getElementById('status-working') !== null) ) {
    // Set the status box
    statusbox = document.getElementById("status-working");

    // Get the UUID of the log file
    var linearray = statusline.split("###");
    var itemuuid = linearray[1];

    // Start the loop checking for log file output
    if ( doingstatus != itemuuid ) {
      doingstatus = itemuuid;
      setTimeout(startLogStatus(doingstatus), 500);
    }
  }
}

function startLogStatus()
{
  // Request the log file for this task
  var eventjson = '{ "namespace":"rpc", "name":"dispatcher", "id":"logrequest", "args":["log ' + doingstatus + '"] }';
  doSend(eventjson);
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

function replaceStatusBox(message)
{
  var header = '<br><table class="status" style="width:768px"><tr><th>Current Status</th></tr><tr><td>';
  var footer = '</td></tr></table>';
  var formatted = "";

  // Read line by line
  var lines = message.split('\n');
  for(var i = 0;i < lines.length;i++){
    formatted = formatted + lines[i] + "<br>";
  }

  // Set the status box
  statusbox.innerHTML = header + formatted + footer;
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
