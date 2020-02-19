#ifndef ANEMONE_H
#define ANEMONE_H

#include <string>
#include <memory>


#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/config.hpp"
#include "anemone/state.hpp"


class Anemone {
public:
  Anemone(std::string config_path,
          std::shared_ptr<GridDevice>);
  
  void run();

  std::shared_ptr<Config> config;
  std::shared_ptr<IO> io;
  std::shared_ptr<State> state;
};

#endif
