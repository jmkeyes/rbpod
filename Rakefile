# Bring in rake with rake-compiler's support.
require 'rake'
require 'rake/extensiontask'

# Bring in bundler and it's gem tasks.
require 'bundler/setup'
require 'bundler/gem_tasks'

# Bring in RSpec's built-in rake task.
require 'rspec/core/rake_task'

# Bring in RDoc's built-in rake task.
require 'rdoc/task'

# By default, clean, compile and then test.
task :default => [ :compile, :test ]

desc "Compile the native extension."
Rake::ExtensionTask.new do |extension|
  # Some basic configuration.
  extension.name     = 'rbpod'
  extension.ext_dir  = 'ext/rbpod'
  extension.lib_dir  = 'lib/rbpod'

  # Monitor sources for change tracking.
  extension.source_pattern = '*.{c,h}'
end

desc "Run all available RSpec tests."
RSpec::Core::RakeTask.new(:test) do |task|
  task.pattern = 'spec/**/*_spec.rb'
  # Execute ruby with warnings enabled.
  task.ruby_opts = '-w'
end

desc "Build all RDoc documentation."
RDoc::Task.new(:rdoc) do |task|
 task.main = 'README.md'
 task.markup = 'markdown'
 task.rdoc_files.include("README.md", 'lib/**/*.rb', 'ext/**/*.[ch]')
 task.options << "--all"
 task.rdoc_dir = 'doc'
end

desc "Open a console with rbpod preloaded."
task :console => [ :compile ] do
  sh 'bundle console'
end
