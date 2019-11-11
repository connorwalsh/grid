#ifndef GR1_H
#define GR1_H

#include "instrument.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"


class GR1 : public Instrument {
public:
  GR1(Config *config, IO *io) : Instrument(config, io) {};
};

#endif
