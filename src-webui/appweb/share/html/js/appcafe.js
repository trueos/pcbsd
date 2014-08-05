function delConfirm(appname) {
    if (confirm("Are you sure you want to delete " + appname + "?") == true) {
	window.location.href = "";
    }
}

function addConfirm(appname) {
    if (confirm("Are you sure you want to install " + appname + "?") == true) {
	window.location.href = "";
    }
}
