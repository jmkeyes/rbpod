describe RbPod::Playlist do
  context 'with a new playlist' do
    let (:playlist) { RbPod::Playlist.new }

    describe '#initialize' do
      it 'should return a new playlist' do
        playlist.should be_instance_of(RbPod::Playlist)
      end
    end

    describe '#name' do
      it 'should be "New Playlist"' do
        playlist.name.should be_kind_of(String)
        playlist.name.should eq('New Playlist')
      end
    end

    describe '#created_on' do
      it 'should have a creation date' do
        playlist.created_on.should be_kind_of(Time)
      end
    end

    describe '#length' do
      it 'should contain zero tracks' do
        playlist.length.should be_kind_of(Integer)
        playlist.length.should eq(0)
      end
    end

    describe '#tracks' do
      it 'should return a collection of tracks' do
        playlist.tracks.should be_instance_of(RbPod::TrackCollection)
      end

      it 'should have zero tracks within it' do
        playlist.tracks.size.should be_kind_of(Integer)
        playlist.tracks.size.should eq(0)
      end
    end

    describe '#smart?' do
      it 'should not be a smart playlist' do
        playlist.should_not be_smart
      end
    end

    describe '#master?' do
      it 'should not be the master playlist' do
        playlist.should_not be_master
      end
    end

    describe '#podcast?' do
      it 'should not be the podcast playlist' do
        playlist.should_not be_podcast
      end
    end
  end
end
