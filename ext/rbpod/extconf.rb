require 'mkmf'

# Fail immediately if we don't have libgpod-1.0 installed here.
abort "Please install libgpod-1.0" unless pkg_config('libgpod-1.0')

# Unfortunately, rdoc isn't capable of a lot of things.
$defs.push("-DRDOC_CAN_PARSE_DOCUMENTATION=0")

# Provide HAVE_STDDEF_H to the pre-processor.
have_header('stddef.h')

# Provide HAVE_GPOD_ITDB_H to the pre-processor.
have_header('gpod/itdb.h')

create_makefile('rbpod/rbpod')
