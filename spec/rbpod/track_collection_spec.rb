describe RbPod::TrackCollection do
  it_behaves_like RbPod::Collection
  # A track collection only requires a playlist, so we don't need a database.
  let(:playlist) { RbPod::Playlist.new() }

  # Use the new playlist as a parent for this track collection.
  let(:collection) { described_class.new(playlist) }

  describe '#playlist' do
    it 'should return the parent playlist' do
      collection.playlist.should eq(playlist)
      collection.playlist.should be_kind_of(RbPod::Playlist)
    end
  end
end
