#include "anemone/io/midi/device/rtmidi.hpp"


RTMidi::RTMidi() {
  try {
    in = std::make_unique<RtMidiIn>();
  } catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  try {
    out = std::make_unique<RtMidiOut>();
  } catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }
}


void RTMidi::connect_in(std::string port_name) {
  unsigned int port;
  ports = get_port_names();
  
  try {
    port = ports.in.at(port_name);
  } catch (std::out_of_range &error) {
    std::cout << "port '" << port_name << "' unavailable!\n";
    exit( EXIT_FAILURE );
  }

  in->openPort(port);
}

void RTMidi::connect_out(std::string port_name) {
  unsigned int port;
  ports = get_port_names();

  try {
    port = ports.out.at(port_name);
  } catch (std::out_of_range &error) {
    std::cout << "port '" << port_name << "' unavailable!\n";
    exit( EXIT_FAILURE );
  }

  out->openPort(port);
}

midi_ports_t RTMidi::get_port_names() {
  if (ports.in.size() == 0 && ports.out.size() == 0) {
    ports = {
             .in  = get_input_port_names(),
             .out = get_output_port_names(),
    };    
  }

  return ports;
}

std::map<std::string, unsigned int> RTMidi::get_input_port_names() {
  // make sure there are ports available
  unsigned int n_ports = in->getPortCount();
  if (n_ports == 0) {
    std::cout << "no midi input ports available!\n";
    exit( EXIT_FAILURE );
  }

  std::map<std::string, unsigned int> port_names;
  
  for (unsigned int i = 0; i < n_ports; i++) {
    try {
      port_names[in->getPortName(i)] = i;
    } catch (RtMidiError &error) {
      error.printMessage();
    }
  }

  return port_names;
}

std::map<std::string, unsigned int> RTMidi::get_output_port_names() {
  // make sure there are ports available
  unsigned int n_ports = out->getPortCount();
  if (n_ports == 0) {
    std::cout << "no midi output ports available!\n";
    exit( EXIT_FAILURE );
  }

  std::map<std::string, unsigned int> port_names;
  
  for (unsigned int i = 0; i < n_ports; i++) {
    try {
      port_names[out->getPortName(i)] = i;
    } catch ( RtMidiError &error ) {
      error.printMessage();
    }
  }

  return port_names;  
}

void RTMidi::listen() {
  in->setCallback(this->callback_wrapper, this);
}

void RTMidi::emit(midi_event_t event) {
  out->sendMessage(event.data);
}

void RTMidi::callback_wrapper(double deltatime, std::vector<unsigned char> *msg, void *user_data) {
  RTMidi *this_rtmidi = (RTMidi *)user_data;

  midi_event_t event = this_rtmidi->transform(deltatime, msg);

  this_rtmidi->broadcast(event);
}

midi_event_t RTMidi::transform(double timestamp, std::vector<unsigned char> *msg) {
  return midi_event_t(timestamp, msg);
}