#
#
# Copyright (c) 2019 IBM Corp.
#
# This program is free software; you can redistribute it and/or modify it
#  under the terms of version 2.1 of the GNU Lesser General Public License
#  as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
#  License for more details.
#
# You should have received a copy of the GNU General Public License along
#  with this program; if not, write to:
#    Free Software Foundation, Inc.
#    51 Franklin Street, Fifth Floor
#    Boston, MA 02110-1301, USA.
#
#

lib_LTLIBRARIES = libtape-dummy.la
libdir = @libdir@/ltfs

AM_LIBTOOLFLAGS = --tag=disable-static

libtape_dummy_la_SOURCES = dummy.c
libtape_dummy_la_DEPENDENCIES =
libtape_dummy_la_LIBADD =
libtape_dummy_la_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src
libtape_dummy_la_LDFLAGS = -avoid-version -module

install-exec-hook:
	for f in $(lib_LTLIBRARIES); do rm -f $(libdir)/$$f; done
