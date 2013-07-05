#!/bin/sh

# $FreeBSD: user/cperciva/freebsd-update-build/tools/build-upgrade-patches.sh 247826 2013-03-05 00:52:14Z cperciva $

if [ $# -lt 3 ]; then
	echo "usage: $0 WWWDIR ARCH TARGETREL OLDRELS"
	exit 1
fi

WWWDIR=$1
ARCH=$2
TARGETREL=$3
OLDRELS=$4

mkdir -p ${WWWDIR}/to-${TARGETREL}/${ARCH}/bp/
for V in ${OLDRELS}; do
	zcat ${WWWDIR}/${V}/${ARCH}/m/* |
	    cut -f 3,4,9 -d '|' |
	    fgrep '|f|' |
	    cut -f 1,3 -d '|' |
	    sort -u |
	    grep -E '\|[0-9a-f]{64}' |
	    lam -s "${V}|" -
done |
    sort -k 2,2 -t '|' > hashtab

zcat ${WWWDIR}/${TARGETREL}/${ARCH}/m/* |
    cut -f 3,4,9 -d '|' |
    fgrep '|f|' |
    cut -f 1,3 -d '|' |
    sort -u |
    grep -E '\|[0-9a-f]{64}' |
    lam -s "${TARGETREL}|" - |
    sort -k 2,2 -t '|' |
    join -1 2 -2 2 -t '|' hashtab - |
    cut -f 2- -d '|' |
    sort -k 2,2 -t '|' |
    tr '|' ' ' |
    while read OR OH NR NH; do
	if [ -f ${WWWDIR}/to-${TARGETREL}/${ARCH}/bp/${OH}-${NH} ]; then
		continue
	fi
	if [ ${OH} = ${NH} ]; then
		continue
	fi
	gunzip < ${WWWDIR}/${OR}/${ARCH}/f/${OH}.gz > ${OH}
	gunzip < ${WWWDIR}/${NR}/${ARCH}/f/${NH}.gz > ${NH}
	bsdiff ${OH} ${NH} ${WWWDIR}/to-${TARGETREL}/${ARCH}/bp/${OH}-${NH}
	rm ${OH} ${NH}
    done

rm hashtab
