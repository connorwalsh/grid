#include <spdlog/spdlog.h>

#include "anemone/ui/page.hpp"


PageUI::PageUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();

  // rendered page observable
  auto rendered_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.rendered.get_observable();
              })
    | rx::switch_on_next();

  // page under edit observable
  auto under_edit_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.under_edit.get_observable();
              })
    | rx::switch_on_next();

  // last page observable
  auto last_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.last.get_observable();
              })
    | rx::switch_on_next();
  
  // page in playback observable
  auto in_playback_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.current.get_observable();
              })
    | rx::switch_on_next()  
    | rx::map([state] (granular_step_idx_t granular_step) {
                auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                return granular_to_paged_step(granular_step, page_size).page;
              })
    | rx::distinct_until_changed();
  

  // page ui logic.
  rendered_page_observable.combine_latest(under_edit_page_observable,
                                          last_page_observable,
                                          in_playback_page_observable)
    .subscribe([this] (std::tuple<page_idx_t, page_idx_t, page_idx_t, page_idx_t> t) {
                 auto last_page        = std::get<2>(t);
                 auto page_in_playback = std::get<3>(t);

                 turn_on_led(page_in_playback);

                 if (page_in_playback == 0) {
                   turn_off_led(last_page);
                 } else {
                   turn_off_led(page_in_playback - 1);
                 }
               });
}
