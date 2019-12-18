#include <string>
#include <iostream>

#include <monome.h>

#include "anemone/io/grid/device/monome.hpp"
#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"


void Monome::connect(std::string addr = "/dev/tty.usbserial-m1000843") {
  // TODO (coco|5.11.2019) parameterize monome serial port location.
  // rpi : /dev/ttyUSB0
  // osx : /dev/tty.usbserial-m1000843
  if( !(monome = monome_open(addr.c_str())) ) {
    std::cout << "Could not connect to monome grid!\n";
    exit( EXIT_FAILURE );
  }
  std::cout << "CONNECTED TO MONOME!\n";
}


void Monome::listen() {
  monome_register_handler(monome, MONOME_BUTTON_DOWN, this->callback_wrapper, this);
  monome_register_handler(monome, MONOME_BUTTON_UP, this->callback_wrapper, this);

  monome_event_loop(monome);
}


void Monome::turn_off(grid_coordinates_t c) {
  monome_led_off(monome, c.x, c.y);
}


void Monome::turn_on(grid_coordinates_t c) {
  monome_led_on(monome, c.x, c.y);
}


void Monome::set(grid_coordinates_t c, unsigned int intensity) {
  monome_led_level_set(monome, c.x, c.y, intensity);
}


void Monome::callback_wrapper(const monome_event_t *m_event, void *user_data) {
  Monome *this_monome = (Monome *)user_data;

  grid_device_event_t event = this_monome->transform(m_event);

  this_monome->broadcast(event);

}


grid_device_event_t Monome::transform(const monome_event_t *m_event) {
  GridDeviceEvent event_type;

  switch (m_event->event_type) {
  case MONOME_BUTTON_DOWN:
    event_type = GridDeviceEvent::ButtonDown;
    break;
  case MONOME_BUTTON_UP:
    event_type = GridDeviceEvent::ButtonUp;
    break;
  default:
    event_type = GridDeviceEvent::ButtonUp;
    break;
  };

  return {{ .x  = m_event->grid.x,
            .y  = m_event->grid.y },
          .type = event_type
  };
}