#!/bin/sh


sed -i.bak "s,/usr/bin/env python,${PBI_PROGDIRPATH}/bin/python,g" ${PBI_PROGDIRPATH}/bin/gretools
sed -i.bak 's|"${prefix}/share|gretools.prefix_dir+"/share|g' ${PBI_PROGDIRPATH}/bin/gretools
