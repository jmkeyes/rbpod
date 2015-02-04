require 'rspec'
require 'rbpod'
require 'tmpdir'

require 'support/shared_contexts'

SPEC_DIR = File.dirname(__FILE__)

def within_temporary_directory(&block)
  Dir.mktmpdir(&block)
end

def sample_data_file(file)
  File.join(SPEC_DIR, 'data', file)
end
