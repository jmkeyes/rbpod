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

```ruby
database = RbPod::Database.new('/mnt/ipod/') # => #<RbPod::Database:0xdeadbeef>

database.version       # => 42
database.mountpoint    # => "/mnt/ipod/"
database.filename      # => "/mnt/ipod/iPod_Control/iTunes/iTunesDB"
database.synchronized? # => true
```
Loading a database is common, so you can also use a shortcut:

```ruby
database = RbPod('/mnt/ipod/')
```

If you'd like to create a new, blank database you can do that too:

```ruby
database = RbPod::Database.create!('/tmp/ipod-blank') # => #<RbPod::Database:0xdeadbeef>
```

### RbPod::Device

The device (if any) that backs the database can be interrogated:

```ruby
device = database.device # => #<RbPod::Device:0xdeadbeef>

# Model name, number, and capacity.
device.capacity     # => 4.0
device.generation   # => "Nano Video (3rd Gen.)"
device.model_name   # => "Nano (Silver)"
device.model_number # => "A978"

# Feature support.
device.supports_photos?         # => true
device.supports_videos?         # => true
device.supports_artwork?        # => true
device.supports_podcasts?       # => true
device.supports_chapter_images? # => true

# Reading/writing SysInfo. Set a key to nil to erase it.
device['ModelNumStr'] # => "xA978"
device['PotsdamConf45'] = "Kilroy Was Here"
device.save!
```

### RbPod::PlaylistCollection

You can get direct access to the playlists on the device:

```ruby
database.playlists # => #<RbPod::PlaylistCollection:0xdeadbeef>

# For a list of all the names of playlists on the iPod:
database.playlists.map(&:name) # => ["iPod", "Podcasts", "Recently Added"]

# For direct access to the master playlist:
database.playlists.master # => #<RbPod::Playlist:0xdeadbeef>

# For direct access to the podcast playlist:
database.playlists.podcast # => #<RbPod::Playlist:0xdeadbeef>
```

### RbPod::Playlist

All playlists support a variety of methods:

```ruby
playlist = database.playlists.master

playlist.name       # => "iPod"
playlist.length     # => 400
playlist.created_on # => 2008-04-05 08:47:46 -0700

playlist.master?  # => true
playlist.smart?   # => false
playlist.podcast? # => false

playlist.tracks # => #<RbPod::TrackCollection:0xdeadbeef>
```
### RbPod::TrackCollection

All playlists contain a `tracks` method which returns an `RbPod::TrackCollection`:

```ruby
tracks = database.playlists.master.tracks

tracks.playlist # => #<RbPod::Playlist:0xdeadbeef>
tracks.length   # => 400

tracks.first # => #<RbPod::Track:0xdeadbeef>
tracks[0]    # => #<RbPod::Track:0xdeadbeef>
```

### RbPod::Track

```ruby
track = database.playlists.master.tracks.first

track.artist       # => "Steppenwolf"
track.title        # => "Born To Be Wild"
track.album        # => "All Time Greatest Hits Remastered"
track.file_type    # => "MPEG audio file"
track.transferred? # => true
````

### RbPod::Error

If anything goes belly up at run time, an `RbPod::Error` exception should be thrown with a detailed message.

## Todo

 * The RSpec tests need a lot of work and significantly more coverage, especially for integration.
 * The documentation could be spiced up a bit. It's dry and doesn't provide too much useful information.
 * The libgpod calls do not unlock the GVL; the library will block in multi-threaded environments.
 * The extension is oblivious to string encoding; it's built for Ruby >= 1.9.3.
 * Consider switching to libid3tag0 from libtagc0.

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
