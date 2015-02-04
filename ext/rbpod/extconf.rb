require 'mkmf'

# Fail immediately if we don't have libgpod-1.0 installed here.
abort "Please install libgpod-1.0" unless pkg_config('libgpod-1.0')

puts "Taglib is not installed; ID3 tag parsing disabled." unless pkg_config('taglib_c')

# Unfortunately, rdoc isn't capable of a lot of things.
$defs.push("-DRDOC_CAN_PARSE_DOCUMENTATION=0")

# Compile with debuging symbols if enabled.
if enable_config('debug')
  CONFIG['debugflags'] << ' -ggdb3 -O0'
end

# Provide HAVE_STDDEF_H to the pre-processor.
have_header('stddef.h')

# Provide HAVE_SYS_STAT_H to the pre-processor.
have_header('sys/stat.h')

# Provide HAVE_GPOD_ITDB_H to the pre-processor.
have_header('gpod/itdb.h')

# Provide HAVE_TAGLIB_TAG_C_H to the pre-processor.
have_header('taglib/tag_c.h')

create_makefile('rbpod/rbpod')
