#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include "instrument.hpp"
#include "ms_20.hpp"

class InstrumentFactory {
public:
  InstrumentFactory() {
    add<MS20>("ms20");
  };

  Instrument *create(std::string name) {
    return constructor_map[name]();
  };
  
private:
  /*
    this is a map of instrument constructors keyed by their string names.
  */
  std::map<std::string, Instrument*(*)()> constructor_map;

  /*
    a template function for instantiating derived instrument classes.
  */
  template<typename T> static Instrument *construct() {
    return new T;
  };

  /*
    resgisters instruments with the global instrument factory.

    each instrument class must be registered.
  */
  template<typename T> void add(std::string name) {
    constructor_map[name] = &(construct<T>);
  };
};

#endif