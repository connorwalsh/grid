#ifndef ANIMATION_ANIMATOR_H
#define ANIMATION_ANIMATOR_H

#include <map>
#include <algorithm>

#include <monome.h>
#include <boost/thread.hpp>

#include "types.hpp"
#include "../io/io.hpp"
#include "../sequencer/types.hpp"
#include "../config/mappings/types.hpp"


class Animator {
public:
  // TODO make frame period configurable via constructor
  Animator(IO *io) : io(io) {};
  
  void run() {
    if (is_running) return; // only run once
    
    animation_thread = boost::thread(&Animator::animation_loop, this);
    is_running = true;
  };

  void add(waveform w, mapping_coordinates_t c) {
    // before adding the waveform, lets update the periods to be aligned
    // with the frame period of the animation loop.
    // NOTE: since the periods are constrained to be integers, this will force the periods
    // of the waveform components (modulator, pwm) to be integer multiples of the frame period.
    w.modulator.period = std::max(static_cast<int>(w.modulator.period / frame_period), 1);
    w.pwm.period = std::max(static_cast<int>(w.pwm.period / frame_period), 1);
    
    animations[c] = new WaveformWrapper(w);
  };

  void remove(mapping_coordinates_t c) {
    animations.erase(c);
  };
  
private:
  IO *io;
  bool is_running = false;
  int frame_period = 50;  // milliseconds per cycle
  boost::thread animation_thread;
  std::map<mapping_coordinates_t, WaveformWrapper*> animations;
  unsigned int t = 0;
  
  void animation_loop() {
    while (true) {
      auto tick = Clock::now();
      
      // iterate through animations, compute them, broadcast to monome!
      for (auto i : animations) {
        int intensity = i.second->compute(t);
        monome_led_level_set(io->output.monome,
                             i.first.x,
                             i.first.y,
                             intensity);
      }

      t++;
      
      auto tock = Clock::now();
      Microseconds remaining_usec = Microseconds(frame_period * 1000) - std::chrono::duration_cast<Microseconds>(tock - tick);

      boost::this_thread::sleep(boost::posix_time::microseconds(remaining_usec.count()));
    }
  };
};

#endif