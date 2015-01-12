require 'rspec'
require 'rbpod'
require 'tmpdir'

require 'support/shared_contexts'

def within_temporary_directory(&block)
  Dir.mktmpdir(&block)
end
