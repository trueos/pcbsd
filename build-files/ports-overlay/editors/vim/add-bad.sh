:
# $FreeBSD: editors/vim/add-bad.sh 302686 2012-08-17 16:08:14Z obrien $

usage() {
	echo 'usage: ${0##*/} <bad patch #s to add to ${BADPATCHES}>'
	exit 1
}
if [ $# -lt 1 ]; then
	usage
fi

sed -i '' "s/^BADPATCHES=.*$/& $*/" Makefile
