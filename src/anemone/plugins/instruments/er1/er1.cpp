#include "anemone/plugins/instruments/er1/er1.hpp"


ER1::ER1::Parameter::Parameter(std::string name)
  : name(name)
{}

bool ER1::ER1::Parameter::is_mapped_to(midi_event_t event) {
  // TODO currently, this only detects cc messages...eventually allow mappings
  // from other types of midi message types.
  return
    event.source  == midi_map.source      &&
    event.data[0] == midi_map.status_byte &&
    event.data[1] == midi_map.control;
}

void ER1::ER1::Parameter::set_current_value(midi_event_t event) {
  // extract cc value from event
  value.current.get_subscriber().on_next(event.data[2]);
}

std::vector<midi_event_t> ER1::ER1::Parameter::get_midi_output() {
  return convert_value_to_midi_events(value.current.get_value());
}

ER1::ER1::Pad::Pad(midi_spn_t spn, midi_channel_t chan)
  : midi({ .channel       = chan,
           .spn           = spn,
           .note_number   = spn_to_num(spn),
           .note_on_data  = midi_note_on(spn, chan, 127),
           .note_off_data = midi_note_off(spn, chan),
    })
{}

ER1::ER1::ER1(std::shared_ptr<Config> config)
  : Plugin(config),
    Instrument(create_instrument(InstrumentName::ER1, config, std::make_shared<ER1Layout>(ER1Layout(config))))
{
  // initialize midi map
  for (auto osc : pads.oscillators) {
    midi_map.note_to_pad[osc->midi.note_number] = osc;

    // set parameters for each oscillator pad
  }

  for (auto cymbal : pads.cymbals) {
    midi_map.note_to_pad[cymbal->midi.note_number] = cymbal;
  }

  for (auto audio : pads.audio_ins) {
    midi_map.note_to_pad[audio->midi.note_number] = audio;
  }

  // set global parameters....
}

std::shared_ptr<Layout> ER1::ER1::get_layout() {
  return layout;
}

std::shared_ptr<StateController> ER1::ER1::make_controller(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<Controller>(io, state, shared_from_this());
}

std::shared_ptr<LayoutUI> ER1::ER1::make_ui(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<UI>(config, io, state, shared_from_this());
}
