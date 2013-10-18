# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'rbpod/version'

Gem::Specification.new do |spec|
  spec.name          = "rbpod"
  spec.version       = RbPod::VERSION

  spec.authors       = ["Joshua M. Keyes"]
  spec.email         = ["joshua.michael.keyes@gmail.com"]

  spec.summary       = %q{Ruby bindings to the libgpod library.}
  spec.description   = %q{Lightweight native bindings to the libgpod library.}
  spec.homepage      = "https://github.com/jmkeyes/rbpod"
  spec.license       = 'MIT'

  spec.files         = `git ls-files`.split($/)
  spec.extensions    = ["ext/rbpod/extconf.rb"]
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ['lib', 'ext']

  spec.add_development_dependency 'bundler', '~> 1.3'
  spec.add_development_dependency 'pkg-config', '~> 1.1.4'
  spec.add_development_dependency 'rake'
end
