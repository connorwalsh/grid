/**
 * @file   types/instrument/part.hpp
 * @brief  Part related types.
 * @author coco
 * @date   2020-02-20
 *************************************************/


#ifndef ANEMONE_TYPES_INSTRUMENT_PART_H
#define ANEMONE_TYPES_INSTRUMENT_PART_H


#include <memory>
#include <vector>

#include "anemone/rx.hpp"

#include "anemone/types/controls/ppqn.hpp"
#include "anemone/types/instrument/page/page.hpp"
#include "anemone/types/instrument/step/step.hpp"
#include "anemone/types/instrument/sequence/sequence.hpp"


/// @brief type alias for part index.
typedef unsigned int part_idx_t;

/// @brief type alias for part index.
typedef unsigned int bank_idx_t;

/// @brief bank relative part index.
///
/// @remark
/// since the parts are bucketed into banks, this identifies the part index
/// relative to the bank it is contained in. the conversion from part_idx_t
/// to part_bank_idx_t requires that we know the size of each bank, which is
/// only determinable given a runtime configuration of how the grid layout is
/// organized.
struct part_bank_idx_t {
  unsigned int part;
  unsigned int bank;
};


/// @brief Part class for representing a part.
class Part {
public:
  Part(part_idx_t);

  part_idx_t id;
  // TODO inntroduce bank_relative_part_idx_t?

  struct Ppqn {
    rx::behavior<PPQN> current;
    rx::behavior<PPQN> previous;
    rx::behavior<PPQN> next;
    rx::behavior<bool> pending_change;
  };

  struct Page {
    rx::behavior<page_idx_t> rendered;
    rx::behavior<page_idx_t> under_edit;
    rx::behavior<page_idx_t> last;
    rx::behavior<bool>       follow_cursor;
  };

  // TODO maybe refactor this to be on Instrument innstead of each part....
  // Why is it on each part? is there a specific reasonn it needs to be on each part?
  // well...if we are editing a part not in playback...we might want the transport buttons
  // to tell us that this part is not actually playing. so maybe no refactor...
  struct Transport {
    rx::behavior<bool> is_playing;
    rx::behavior<bool> is_paused;
    rx::behavior<bool> is_stopped;

    rx::behavior<bool> is_stopping;
    rx::behavior<bool> is_transitioning;
    rx::behavior<bool> is_about_to_start;
    rx::behavior<bool> is_about_to_unpause;

    // since pausing a part may leave its cursor not 'on the beat', we must
    // keep track of how much 'off the beat' it has been paused. this allows
    // us to resume a paused part appropriately so that, once resumed, it is
    // still aligned with played 'on the beat'.
    rx::behavior<int> pulse_pause_offset;
  };

  struct Step {
    rx::behavior<granular_step_idx_t> current;
    // rx::behavior<paged_step_idx_t>    current_page_relative = { .page = 0, .step = 0 }; // TODO remove this redundancy!???
    rx::behavior<paged_step_idx_t>    last;
    rx::behavior<bool>                show_last;

    /// @brief update the current step.
    void update_current(granular_step_idx_t);
  };
  
  Ppqn ppqn;
  Page page;
  Transport transport;
  Step step;
  Sequence sequence;
  bool unsaved;
};

#endif
