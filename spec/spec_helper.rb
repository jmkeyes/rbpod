require 'rspec'
require 'rbpod'
require 'tmpdir'

RSpec.configure do |config|
  config.color_enabled = true
  config.formatter     = 'documentation'
end

def within_temporary_directory(&block)
  Dir.mktmpdir(&block)
end
