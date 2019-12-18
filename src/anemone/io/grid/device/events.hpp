#ifndef IO_GRID_DEVICE_EVENTS_H
#define IO_GRID_DEVICE_EVENTS_H

#include "anemone/io/grid/device/coordinates.hpp"


enum class GridDeviceEvent { ButtonUp, ButtonDown };

struct grid_device_event_t : public grid_coordinates_t {
  GridDeviceEvent type;

  static grid_device_event_t make(unsigned int xi, unsigned int yi, GridDeviceEvent e) {
    return {{
             .x = xi,
             .y = yi
             },
            .type = e,
    };
  }
};

#endif