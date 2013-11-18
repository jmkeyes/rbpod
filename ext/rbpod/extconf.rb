#!/usr/bin/env ruby

require 'mkmf'

abort "Please install libgpod-1.0" unless pkg_config('libgpod-1.0')

# Unfortunately, rdoc isn't capable of a lot of things.
$defs.push("-DRDOC_CAN_PARSE_DOCUMENTATION=0")

create_makefile('rbpod/rbpod')
