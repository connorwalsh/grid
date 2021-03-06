#ifndef ACTIONS_DISPATCHER_H
#define ACTIONS_DISPATCHER_H

#include <vector>
#include <memory>

#include "anemone/action/types.hpp"
#include "anemone/rx/root.hpp"

// TODO move this into rx!
class Dispatcher {
public:
  Dispatcher(std::shared_ptr<rx::Root<action_t, high_freq_action_t> >);

  void connect();
  void hydrate();
  
  void dispatch(const action_t& action);
  void dispatch(action_t&& action);
  void dispatch(const high_freq_action_t& action);
  void dispatch(high_freq_action_t&& action);
  void dispatch(std::vector<action_t> actions);

  // syncrhonous dispatch, blocks until action has been processed.
  void dispatch_wait  (action_t action);

private:
  std::shared_ptr<rx::Root<action_t, high_freq_action_t> > root;
};

#endif
