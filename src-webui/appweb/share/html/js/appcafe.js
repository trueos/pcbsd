function delConfirm(appname, origin, cmd, target) {
    if (confirm("Are you sure you want to delete " + appname + "?") == true) {
	var newURL = window.location.pathname + window.location.search;
	window.location.href = newURL + "&deleteApp=" + origin + "&deleteAppCmd=" + cmd;
    }
}

function addConfirm(appname, origin, cmd, target) {
    if (confirm("Are you sure you want to install " + appname + "?") == true) {
	var newURL = window.location.pathname + window.location.search;
	window.location.href = newURL + "&installApp=" + origin + "&installAppCmd=" + cmd ;
    }
}

function delJailConfirm(jname) {
    if (confirm("Are you sure you want to delete jail: " + jname + "?") == true) {
	var newURL = window.location.pathname
	window.location.href = newURL + "?p=jails&deleteJail=" + jname;
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
