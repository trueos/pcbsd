
// HTML to replace add/remove buttons with during queue action of pkg
var appWorkingHTML= "<div id='item-working'><a title=\"AppCafe is working. Click for details\" href=\"?p=dispatcher&ref=appinfo\"><img align=absmiddle height=32 width=32 src=\"../images/working.gif\" title=\"AppCafe is working. Click for details\"></a></div>";
var appWorkingHTMLFloat = "<div id='item-working'><a title=\"AppCafe is working. Click for details\" href=\"?p=dispatcher&ref=appinfo\"><img style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" height=22 width=22 src=\"../images/working.gif\" title=\"AppCafe is working. Click for details\"></a></div>";

// HTML to replace add/remove buttons with during queue action of plugin
var pluginWorkingHTML = "<div id='item-working'><a title=\"AppCafe is working. Click for details\" href=\"?p=dispatcher&ref=plugininfo\"><img align=absmiddle height=32 width=32 src=\"../images/working.gif\" title=\"AppCafe is working. Click for details\"></a></div>";
var pluginWorkingHTMLFloat = "<div id='item-working'><a title=\"AppCafe is working. Click for details\" href=\"?p=dispatcher&ref=plugininfo\"><img style=\"background-color: Transparent;background-repeat:no-repeat;border: none;float:right;\" height=22 width=22 src=\"../images/working.gif\" title=\"AppCafe is working. Click for details\"></a></div>";

function delConfirm(appname, origin, cmd, divid) {
    if (confirm("Are you sure you want to delete " + appname + "?") == true) {
        // This function is in notifications.js
        queueDeleteApp(origin, cmd);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = appWorkingHTML;
    }
}

function addConfirm(appname, origin, cmd, divid) {
    if (confirm("Are you sure you want to install " + appname + "?") == true) {
        // This function is in notifications.js
        queueInstallApp(origin, cmd);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = appWorkingHTML;
    }
}

function delConfirmTiny(appname, origin, cmd, divid) {
    if (confirm("Are you sure you want to delete " + appname + "?") == true) {
        // This function is in notifications.js
        queueDeleteApp(origin, cmd);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = appWorkingHTMLFloat;
    }
}

function addConfirmTiny(appname, origin, cmd, divid) {
    if (confirm("Are you sure you want to install " + appname + "?") == true) {
        // This function is in notifications.js
        queueInstallApp(origin, cmd);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = appWorkingHTMLFloat;
    }
}

function delJailConfirm(name) {
    if (confirm("Are you sure you want to delete jail: " + name + "?") == true) {
	var newURL = window.location.pathname;
	window.location.href = newURL + "?p=jails&deleteJail=" + name;
    }
}

function delAppConfirm(name, origin, ioid, divid) {
    if (confirm("Are you sure you want to delete plugin: " + name + "?") == true) {
        // This function is in notifications.js
        queueDeletePlugin(origin, ioid);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = pluginWorkingHTML;
    }
}

function addAppConfirm(name, origin, ghurl, divid) {
    if (confirm("Are you sure you want to install the plugin: " + name + "?") == true) {
        // This function is in notifications.js
        queueInstallPlugin(origin, ghurl);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = pluginWorkingHTML;
    }
}

function delAppConfirmTiny(name, origin, ioid, divid) {
    if (confirm("Are you sure you want to delete plugin: " + name + "?") == true) {
        // This function is in notifications.js
        queueDeletePlugin(origin, ioid);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = pluginWorkingHTMLFloat;
    }
}

function addAppConfirmTiny(name, origin, ghurl, divid) {
    if (confirm("Are you sure you want to install the plugin: " + name + "?") == true) {
        // This function is in notifications.js
        queueInstallPlugin(origin, ghurl);
        replaceid = document.getElementById(divid);
	replaceid.innerHTML = pluginWorkingHTMLFloat;
    }
}

function togglePBIMode() {
    var URL = window.location.pathname + window.location.search;
    var viewUrl = addParameter(URL, "allPBI", ! document.getElementById("pbiswitch").checked);
    window.location.href = viewUrl;
}

function addParameter(url, param, value) {
    // Using a positive lookahead (?=\=) to find the
    // given parameter, preceded by a ? or &, and followed
    // by a = with a value after than (using a non-greedy selector)
    // and then followed by a & or the end of the string
    var val = new RegExp('(\\?|\\&)' + param + '=.*?(?=(&|$))'),
        parts = url.toString().split('#'),
        url = parts[0],
        hash = parts[1]
        qstring = /\?.+$/,
        newURL = url;

    // Check if the parameter exists
    if (val.test(url))
    {
        // if it does, replace it, using the captured group
        // to determine & or ? at the beginning
        newURL = url.replace(val, '$1' + param + '=' + value);
    }
    else if (qstring.test(url))
    {
        // otherwise, if there is a query string at all
        // add the param to the end of it
        newURL = url + '&' + param + '=' + value;
    }
    else
    {
        // if there's no query string, add one
        newURL = url + '?' + param + '=' + value;
    }

    if (hash)
    {
        newURL += '#' + hash;
    }

    return newURL;
}

function goto(form) {
  var index=form.jailSelect.selectedIndex;
  location=form.jailSelect.options[index].value;
}

