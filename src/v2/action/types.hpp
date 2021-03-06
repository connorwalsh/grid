#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#include <variant>

#include "anemone/action/sequencer.hpp"
#include "anemone/action/steps.hpp"
#include "anemone/action/play_pause.hpp"


namespace action {
  // used to initialize state tree and all connected components
  // this should always catch the default reducer branch to return
  // the default state.
  struct hydrate_state {};
}

using action_t = std::variant<action::hydrate_state,
                              action::bpm_updated,
                              action::instrument_played_or_paused>;

using high_freq_action_t = std::variant<action::step_cursor_updated,
                                        action::step_updated>;

#endif
