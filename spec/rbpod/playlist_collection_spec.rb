describe RbPod::PlaylistCollection do
  it_behaves_like RbPod::Collection
  # Create a temporary database for the test.
  include_context 'a new database'

  # Use it as a parent to the playlist collection.
  let (:collection) { RbPod::PlaylistCollection.new(database) }

  describe '#database' do
    it 'should return the parent database' do
      collection.database.should eq(database)
      collection.database.should be_kind_of(RbPod::Database)
    end
  end

  describe '#master' do
    it 'should return a playlist' do
      collection.master.should be_kind_of(RbPod::Playlist)
    end

    it 'should be marked as the master playlist' do
      collection.master.should be_master
    end
  end

  describe '#podcast' do
    context 'on a new database' do
      it 'should return nil' do
        collection.podcast.should be_nil
      end
    end
  end

  describe '#include?' do
    it 'should detect the master playlist' do
      collection.include?(collection.master).should be true
    end

    it 'should not detect a new playlist' do
      anonymous_playlist = RbPod::Playlist.new()
      collection.include?(anonymous_playlist).should be false
    end
  end

  let (:playlist) { RbPod::Playlist.new() }

  describe '#insert' do
    it 'should add a playlist to the database' do
      collection.insert(playlist)
      collection.database.save!
      collection.include?(playlist).should be true
    end
  end

  describe '#remove' do
    it 'should remove a playlist from the database' do
      collection.insert(playlist)
      collection.database.save!
      collection.remove(playlist)
      collection.database.save!
      collection.include?(playlist).should be false
    end
  end
end
