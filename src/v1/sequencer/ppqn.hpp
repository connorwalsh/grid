#ifndef SEQUENCER_PPQN_H
#define SEQUENCER_PPQN_H

#include "constants.hpp"
#include "../io/io.hpp"
#include "../animation/animator.hpp"
#include "../config/config.hpp"


// Ppqn maintains the state and ui rendering of the ppqn (pulse per quarter note) panel
// for each part.
class Ppqn {
public:
  int current = 8;
  int next;
  bool pending_change = false;


  Ppqn(IO *io, Animator *animation, Config *config)
    : io(io), animation(animation), config(config) {};

  
  // render the ppqn selection panel in the ui.
  void render() {
    int current_idx = index_from_ppqn(current);
    int next_idx = index_from_ppqn(next);
    mapping_coordinates_t current_coords = config->mappings.ppqn.get_coordinates_from_sequential_index(current_idx);
    mapping_coordinates_t next_coords = config->mappings.ppqn.get_coordinates_from_sequential_index(next_idx);

    // remove all animations from ppqn panel and set to off intensity
    animation->remove(config->mappings.ppqn, led.off);

    if (pending_change) animation->add(led.next, next_coords);
    
    // turn on the current ppqn
    monome_led_on(io->output.monome, current_coords.x, current_coords.y);
  };

  // set the next ppqn from the provided ppqn index and re-render ui.
  //
  // this doesn't change the current ppqn but updates the ppqn which will be
  // set next. this method is usually called from within the event handlers thread.
  void set_next(int idx) {
    next = ppqn_from_index(idx);
    pending_change = true;
  }

  // sets the current ppqn using the next ppqn and re-renders.
  //
  // we assume that next has been set already using set_next method. it is worth noting
  // that this method variant is typically called from within a different thread as the
  // set_next method. this method is called from the sequencer scheduler thread usually.
  void set() {    
    set(index_from_ppqn(next));
  }

  // sets the current ppqn using the provided ppqn index and re-renders.
  void set(int idx) {      
    pending_change = false;
    current = ppqn_from_index(idx);
  };
  
private:
  IO *io;
  Config *config;
  Animator *animation;

  // led brightness and animation configuration for ppqn panel
  struct {
    int off = 0;
    int current = 15;
    waveform next = {.amplitude.max = 9,
                     .modulator = { .type = Unit },
                     .pwm = { .duty_cycle = 0.1, .period = 100, .phase = 0 }
    };
  } led;
  
  int ppqn_from_index(int idx) {
    switch (idx) {
    case 0:
      return constants::PPQN::One;
    case 1:
      return constants::PPQN::Two;
    case 2:
      return constants::PPQN::Four;
    case 3:
      return constants::PPQN::Eight;
    case 4:
      return constants::PPQN::Sixteen;
    case 5:
      return constants::PPQN::ThirtyTwo;
    case 6:
      return constants::PPQN::SixtyFour;
    default:
      return constants::PPQN::Eight;
    }
  }

  int index_from_ppqn(int ppqn) {
    switch (ppqn) {
    case constants::PPQN::One:
      return 0;
    case constants::PPQN::Two:
      return 1;
    case constants::PPQN::Four:
      return 2;
    case constants::PPQN::Eight:
      return 3;
    case constants::PPQN::Sixteen:
      return 4;
    case constants::PPQN::ThirtyTwo:
      return 5;
    case constants::PPQN::SixtyFour:
      return 6;
    default:
      return 0;
    }
  }
};

#endif
