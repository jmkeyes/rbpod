describe RbPod::TrackCollection do
  it_behaves_like RbPod::Collection do
    # A track collection only requires a playlist, so we don't need a database.
    let(:playlist) { RbPod::Playlist.new('New Playlist') }

    # Use the new playlist as a parent for this track collection.
    let(:collection) { described_class.new(playlist) }
  end
end
