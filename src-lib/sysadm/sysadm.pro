TEMPLATE = subdirs
CONFIG += recursive

SUBDIRS+= library binary

#Make sure to list the library as a requirement for the others (for parallellized builds)
binary.depends = library

