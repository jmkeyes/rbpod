describe RbPod::Database do
  context 'Without a new database' do
    describe '.create!' do
      it 'should create an empty default database and load it' do
        within_temporary_directory do |directory|
          RbPod::Database.create!(directory).should be_instance_of(RbPod::Database)
        end
      end

      it 'should throw an error when given an invalid mount point.' do
        proc { RbPod::Database.create!('') }.should raise_error RbPod::Error
      end

      it 'should throw an error when given an invalid device name.' do
        within_temporary_directory do |directory|
          proc { RbPod::Database.create!(directory, '') }.should raise_error RbPod::Error
        end
      end

      it 'should throw an error when given an invalid model number.' do
        within_temporary_directory do |directory|
          proc { RbPod::Database.create!(directory, nil, '') }.should raise_error RbPod::Error
        end
      end
    end

    describe '#initialize' do
      it 'should fail to load a directory without an iPod structure' do
        within_temporary_directory do |directory|
          proc { @database = RbPod::Database.new(directory) }.should raise_error RbPod::Error
        end
      end
    end
  end

  context 'With a new database' do
    include_context 'a new database'

    describe '#initialize' do
      it 'should return a Database instance' do
        database.should be_instance_of(RbPod::Database)
      end
    end

    describe '#mountpoint' do
      it 'should match the mount point it was loaded from' do
        database.mountpoint.to_s.should == directory
      end
    end

    describe '#filename' do
      it 'should be an existing iTunes database on the file system' do
        File.exists?(database.filename).should be true
      end
    end

    describe '#device' do
      it 'should return a Device instance' do
        database.device.should be_instance_of(RbPod::Device)
      end
    end

    describe '#synchronized?' do
      it 'should be marked as synchronized' do
        database.should be_synchronized
      end
    end

    describe '#playlists' do
      it 'should return a playlist collection' do
        database.playlists.should be_instance_of(RbPod::PlaylistCollection)
      end
    end

    describe '#tracks' do
      it 'should return a track collection' do
        database.tracks.should be_instance_of(RbPod::TrackCollection)
      end
    end
  end
end
