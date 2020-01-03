#include "anemone/io/io.hpp"


IO::IO(std::shared_ptr<Config> config,
      std::shared_ptr<GridDevice> grid_device,
      std::shared_ptr<MidiDevice> midi_device,
      layout_initializer_list layouts) {
  grid = std::make_shared<Grid>(config, grid_device, layouts);
  midi = std::make_shared<Midi>(config, midi_device);
}

void IO::connect() {
  grid->connect();
  midi->connect();
}