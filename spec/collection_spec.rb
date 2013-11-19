describe RbPod::Collection do
  before do
    @block = proc { |item| nil }
    @collection = RbPod::Collection.new
  end

  describe '.self' do
    it 'should include Enumerable' do
      @collection.class.should include(Enumerable)
    end
  end

  describe '#initialize' do
    it 'should be a Collection instance' do
      @collection.should be_instance_of(RbPod::Collection)
    end
  end

  describe '#each' do
    it 'should yield to a block and return itself' do
      @collection.each(&@block).should_not be_nil
    end

    it 'should return an Enumerator without a block' do
      @collection.each.should be_kind_of(Enumerator)
    end
  end

  describe '#length' do
    it 'should be zero' do
      @collection.length.should eq(0)
    end

    it 'should be aliased to #size' do
      @collection.should respond_to(:size)
      @collection.size.should eq(0)
    end
  end
end
