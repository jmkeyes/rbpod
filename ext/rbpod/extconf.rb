#!/usr/bin/env ruby

require 'mkmf'

abort "Please install libgpod-1.0" unless pkg_config('libgpod-1.0')

create_makefile('rbpod/rbpod')
