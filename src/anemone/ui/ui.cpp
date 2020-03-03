#include <spdlog/spdlog.h>

#include "anemone/ui/ui.hpp"


UI::UI(std::shared_ptr<Config> config, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : config(config),
    io(io),
    state(state)
{}


void UI::connect() {
  shift = std::make_unique<ShiftUI>(LayoutName::SequencerAndInstrument, GridSectionName::Shift, io, state);
  step_sequence = std::make_unique<StepSequenceUI>(LayoutName::SequencerAndInstrument, GridSectionName::Steps, io, state);
  pages = std::make_unique<PageUI>(LayoutName::SequencerAndInstrument, GridSectionName::Pages, io, state);
  parts = std::make_unique<PartsUI>(LayoutName::SequencerAndInstrument, GridSectionName::Parts, io, state);
  banks = std::make_unique<BanksUI>(LayoutName::SequencerAndInstrument, GridSectionName::Banks, io, state);
  ppqn = std::make_unique<PPQNUI>(LayoutName::SequencerAndInstrument, GridSectionName::PPQN, io, state);
  play_pause = std::make_unique<PlayPauseUI>(LayoutName::SequencerAndInstrument, GridSectionName::PlayPause, io, state);
  stop = std::make_unique<StopUI>(LayoutName::SequencerAndInstrument, GridSectionName::Stop, io, state);

  spdlog::info("  connected -> ui");
}
