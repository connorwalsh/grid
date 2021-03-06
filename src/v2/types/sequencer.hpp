/**
 * @file   types/sequencer.hpp
 * @brief  Sequencer Related Types
 * @author coco
 * @date   2020-01-17
 *************************************************/


#ifndef TYPES_SEQUENCER_H
#define TYPES_SEQUENCER_H

#include <map>
#include <set>
#include <vector>


namespace types {

  // forward declaration. see anemone/types/midi.hpp
  namespace midi {
    typedef std::vector<unsigned char> data_t;
  }
  
  /// @brief sequence page types.
  namespace page {
    /// @brief type alias for a page index; used to identify pages.
    typedef unsigned int idx_t;
  }

  
  /// @brief sequence step types.
  namespace step {
    /// @brief type alias for sequence relative step index; a sequential id.
    ///
    /// @remark
    /// a step index is used to index a step in a sequence. unlike its granular
    /// counter-part, it represents the index that one actually sees on the sequencer
    /// and can be thought of as the sequential index of an event within the sequence.
    /// This index type is 'sequence' relative because it is agnostic to which page the
    /// index lies on.
    ///
    typedef unsigned int idx_t;

    /// @brief type alias for granular sequence relative indices.
    ///
    /// @remark
    /// granular step indices are used to index into sequence steps and are
    /// used as the active step maintained within a Part as it is advanced by
    /// the sequence scheduler.
    /// these indices are "granular" or "fine-grained" because each step represents
    /// a 1/constants::PPQN_MAX step (the shortest step possible) and, given how
    /// sequences are stored/indexed internally, allows for fluid transitions of a
    /// Part's effective ppqn.
    /// Like its non-granular counterpart, this index type is 'sequence' relative which
    /// means it is agnostic to which page the index lies on.
    ///
    typedef unsigned int granular_idx_t;

    /// @brief type alias for page relative step index.
    ///
    /// @remark
    /// This type is used when we are only concerned with the index of a step relative
    /// to the page it is on.
    ///
    typedef unsigned int page_relative_idx_t;
    
    /// @brief paged step index. provides the page index and page-relative step index.
    ///
    /// @details
    /// 
    struct paged_idx_t {
      page::idx_t               page;
      page_relative_idx_t       step;

      bool operator==(const paged_idx_t& rhs) {
        return
          page == rhs.page &&
          step == rhs.step;
      };

      idx_t to_absolute_idx(unsigned int page_size) {
        return (page * page_size) + step;
      };
    };

    /// @brief namespace encapsulates types related to step events
    namespace event {
      /// @brief Step event protocol.
      enum class protocol_t {
                             /// MIDI protocol.
                             Midi,
                             /// Non-Registered Parameter Number (NRPN).
                             NRPN,
      };

      /// @brief Step event unique identifier.
      ///
      /// @details
      /// for midi events:
      /// `0x<status byte><data 1 byte>`
      /// where,
      ///   `status byte` is the command + channel
      ///   `data 1 byte` is a 0-127 value (such as pitch)
      ///
      typedef unsigned short id_t;

      /// @brief create an id for a step event given a protocol and midi data.
      id_t make_id(protocol_t protocol, types::midi::data_t data);
    }

    /// @brief Step event type.
    struct event_t {
      event::protocol_t   protocol;
      event::id_t         id;
      types::midi::data_t data;

      bool operator <(const event_t &rhs) const {
        return id < rhs.id;
      };
  
    };

    /// @brief create a midi note on step event.
    event_t midi_note_on(std::string note, unsigned int channel, unsigned int velocity);
  }


  /// @brief namespace encapsulates types associated with `sequence`s.
  namespace sequence {
    /// @brief a sequence layer type.
    ///
    /// @details
    /// The sequence layer data structure stores sequencer step events in a map keyed
    /// by a unique event id modulo certain characteristic of the event. In other words,
    /// each layer is a step event which can be distinguished from other step events in
    /// a particular way. Currently, the unique id is computed from the midi channel from
    /// which the event came from, the "type" of midi event (e.g. cc, note, nrpn, etc...),
    /// and the "value" of the event (e.g. the cc, note, or nrpn value, etc...). Structuring
    /// the data this way allows for (1) polyphony (2) efficiently muting/soloing specific
    /// voices.
    ///
    typedef std::map<step::event::id_t, step::event_t> layer_t;
  }

  /// @brief type alias for sequence storing data structure.
  typedef std::map<step::idx_t, sequence::layer_t> sequence_t;

  /// @brief data structure for storing rendered steps
  ///
  /// @todo eventually this must be able to handle layering!
  typedef std::map<page::idx_t, std::set<step::page_relative_idx_t> > rendered_steps_t;

  namespace part {
    /// @brief sequence part index.
    typedef int idx_t;
  }
}

#endif
