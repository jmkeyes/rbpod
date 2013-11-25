shared_context 'a new database' do
  # Create a temporary folder and initialize a database in it.
  before do
    @directory = within_temporary_directory
    @database  = RbPod::Database.create!(@directory)
  end

  # This is a godawful hack, but it's needed to clean up after the tests finish.
  after do
    FileUtils.remove_entry_secure(@directory)
  end

  # Temporary directory for database.
  let(:directory) { @directory }

  # The database instance itself.
  let(:database) { @database }
end
