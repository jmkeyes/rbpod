describe RbPod::PlaylistCollection do
  it_behaves_like RbPod::Collection do
    # Create a temporary database for the test.
    include_context 'a new database'

    # Use it as a parent to the playlist collection.
    let (:collection) { described_class.new(database) }

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
  end
end
