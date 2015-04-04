describe RbPod::Device do
  context 'With a new database' do
    include_context 'a new database'

    let(:device) { database.device }

    describe '#[]' do
      it 'should return a nil value for any key' do
        device['ModelNumStr'].should be_nil
      end

      it 'should return nil with an invalid key' do
        device['NonExistentValue'].should be_nil
      end

      it 'should return nil when unsetting a key' do
        device['NotARealKey'] = nil
        device['NotARealKey'].should be_nil
      end
    end

    describe '#save!' do
      it 'should return nil' do
        device.save!.should eq(nil)
      end
    end

    describe '#uuid' do
      it 'should be nil' do
        device.uuid.should eq(nil)
      end
    end

    describe '#capacity' do
      it 'should be zero' do
        device.capacity.should be_kind_of(Float)
        device.capacity.should eq(0.0)
      end
    end

    describe '#generation' do
      it 'should be "Unknown"' do
        device.generation.should be_kind_of(String)
        device.generation.should eq('Unknown')
      end
    end

    describe '#model_name' do
      it 'should be "Invalid"' do
        device.model_name.should be_kind_of(String)
        device.model_name.should eq('Invalid')
      end
    end

    describe '#model_number' do
      it 'should be "Invalid"' do
        device.model_number.should be_kind_of(String)
        device.model_number.should eq('Invalid')
      end
    end

    %w[photos videos artwork podcasts chapter_images].each do |feature|
      describe "#supports_#{feature}?" do
        it 'should be false' do
          device.send("supports_#{feature}?").should be false
        end
      end
    end
  end
end
