# RbPod

A Ruby interface to the libgpod library.

## Installation

Add this line to your application's Gemfile:

    gem 'rbpod'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install rbpod

## Usage

Functional for most read-only purposes.

    #!/usr/bin/env ruby

    require 'rbpod'

    # Explicitly load a database from the filesystem -- can be a mount point to an iPod.
    database = RbPod::Database.new('/mnt/ipod/') # => #<RbPod::Database:0x8733fa4>

    database.version       # => 42
    database.mountpoint    # => "/mnt/ipod/"
    database.filename      # => "/mnt/ipod/iPod_Control/iTunes/iTunesDB"
    database.synchronized? # => true

    # Device information, including model name, number, and capacity.
    database.device.capacity     # => 4.0
    database.device.generation   # => "Nano Video (3rd Gen.)"
    database.device.model_name   # => "Nano (Silver)"
    database.device.model_number # => "A978"

    # Device feature support detection.
    database.device.supports_photos?         # => true
    database.device.supports_videos?         # => true
    database.device.supports_artwork?        # => true
    database.device.supports_podcasts?       # => true
    database.device.supports_chapter_images? # => true

    # Device SysInfo read/write.
    database.device['ModelNumStr'] # => "xA978"
    database.device['PotsdamConf45'] = "Kilroy Was Here"
    database.device.save!

    # Playlists (includes enumerable)
    database.playlists.map(&:name) # => ["iPod", "Podcasts", "Recently Added"]

    # Track Listing (also enumerable)
    database.tracks.length       # => 400
    database.tracks.first.artist # => "Steppenwolf"
    database.tracks.first.title  # => "Born To Be Wild"

It's alpha quality -- many pieces still need to be implemented. Patches welcome.

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
