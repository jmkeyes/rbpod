describe RbPod::Database do
  context 'Overall' do
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

  context 'Given a blank database' do
    before do
      @directory = within_temporary_directory
      @database  = RbPod::Database.create!(@directory)
    end

    after do
      # This is a godawful hack.
      FileUtils.remove_entry_secure(@directory)
    end

    describe '#initialize' do
      it 'should return a Database instance' do
        @database.should be_instance_of(RbPod::Database)
      end
    end

    describe '#mountpoint' do
      it 'should match the mount point it was loaded from' do
        @database.mountpoint.to_s.should == @directory
      end
    end

    describe '#filename' do
      it 'should be an existing iTunes database on the file system' do
        File.exists?(@database.filename).should be_true
      end
    end

    describe '#device' do
      it 'should return a Device instance' do
        @database.device.should be_instance_of(RbPod::Device)
      end
    end

    describe '#synchronized?' do
      it 'should be marked as synchronized' do
        @database.should be_synchronized
      end
    end

    describe '#playlists' do
      it 'should return a collection' do
        @database.playlists.should be_instance_of(RbPod::Collection)
      end

      it 'should have a single master playlist' do
        @database.playlists.length.should eq(1)
        @database.playlists.first.should be_master_playlist
      end
    end
  end
end
