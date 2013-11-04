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

Functional for most read-only purposes. To get started, add `require 'rbpod'` to your script.

### RbPod::Database

To load a database from the filesystem:

    database = RbPod::Database.new('/mnt/ipod/') # => #<RbPod::Database:0xdeadbeef>

    database.version       # => 42
    database.mountpoint    # => "/mnt/ipod/"
    database.filename      # => "/mnt/ipod/iPod_Control/iTunes/iTunesDB"
    database.synchronized? # => true

If you'd like to create a blank database, you can do that too:

    database = RbPod::Database.create!('/tmp/ipod-blank') # => #<RbPod::Database:0xdeadbeef>

### RbPod::Device

The device (if any) that backs the database can be interrogated:

    # Model name, number, and capacity.
    database.device.capacity     # => 4.0
    database.device.generation   # => "Nano Video (3rd Gen.)"
    database.device.model_name   # => "Nano (Silver)"
    database.device.model_number # => "A978"

    # Feature support.
    database.device.supports_photos?         # => true
    database.device.supports_videos?         # => true
    database.device.supports_artwork?        # => true
    database.device.supports_podcasts?       # => true
    database.device.supports_chapter_images? # => true

    # Reading/writing SysInfo. Set a key to nil to erase it.
    database.device['ModelNumStr'] # => "xA978"
    database.device['PotsdamConf45'] = "Kilroy Was Here"
    database.device.save!

### RbPod::Collection

All methods that return lists return a `Collection` enumerator decorated depending on it's contents:

    database.playlists # => #<RbPod::Collection:0xdeadbeef>

    # For a list of all the names of playlists on the iPod:
    database.playlists.map(&:name) # => ["iPod", "Podcasts", "Recently Added"]

    # For direct access to the master playlist:
    database.playlists.master # => #<RbPod::Playlist:0xdeadbeef>

    # For direct access to the podcasts playlist:
    database.playlists.podcasts # => #<RbPod::Playlist:0xdeadbeef>

### RbPod::Playlist

All playlists support a variety of methods:

    playlist = database.playlists.master

    playlist.name       # => "iPod"
    playlist.length     # => 400
    playlist.created_on # => 2008-04-05 08:47:46 -0700

    playlist.master_playlist?  # => true
    playlist.smart_playlist?   # => false
    playlist.podcast_playlist? # => false

    playlist.tracks # => #<RbPod::Collection:0xdeadbeef>

### RbPod::Track

Tracks also can do a lot, but not complete:

    track = database.playlists.master.tracks.first

    track.artist       # => "Steppenwolf"
    track.title        # => "Born To Be Wild"
    track.album        # => "All Time Greatest Hits Remastered"
    track.file_type    # => "MPEG audio file"
    track.transferred? # => true

### RbPod::Error

If anything goes belly up at run time, an `RbPod::Error` exception should be thrown with a detailed message.

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
