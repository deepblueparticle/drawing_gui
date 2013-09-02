#
# package & author info
#
packagename := drawing-gui
description := a simple gui with a zoom window that you can draw points and lines
major_version := 0
minor_version := 1
tiny_version  := 0
# version := major_version . minor_version # depracated
author := Engin Tola
licence := GPL v2.0 or higher distributed by FSF
#
# add you cpp cc files here
#
sources := main.cc gui.cc
#
# output info
#
installdir := /home/tola/usr
external_sources :=
external_libraries := kortex kortex-ext-opencv
libdir := .
srcdir := .
includedir:= .
#
# custom flags
#
define_flags :=
custom_ld_flags :=
custom_cflags :=
#
# optimization & parallelization ?
#
optimize ?= false
parallelize ?= true
boost-thread ?= false
f77 ?= false
sse ?= true
multi-threading ?= false
profile ?= false
#........................................
specialize := true
platform := native
#........................................
compiler := g++
#........................................
include $(MAKEFILE_HEAVEN)/static-variables.makefile
include $(MAKEFILE_HEAVEN)/flags.makefile
include $(MAKEFILE_HEAVEN)/rules.makefile
