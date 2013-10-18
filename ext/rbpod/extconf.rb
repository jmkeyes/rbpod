#!/usr/bin/env ruby

# Bring in mkmf to build a Makefile.
require 'mkmf'

# Bring in pkg-config so we can find libgpod.
require 'pkg-config'

# Include pkg-config.
include PKGConfig

abort "Please install libgpod-1.0" unless have_package('libgpod-1.0')

create_makefile('rbpod/rbpod')
