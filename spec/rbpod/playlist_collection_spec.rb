describe RbPod::PlaylistCollection do
  it_behaves_like RbPod::Collection do
    # Create a temporary database for the test.
    include_context 'a new database'

    # Use it as a parent to the playlist collection.
    let (:collection) { described_class.new(database) }
  end
end
