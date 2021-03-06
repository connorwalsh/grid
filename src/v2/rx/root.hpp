#ifndef RX_ROOT_H
#define RX_ROOT_H

namespace rx {

  // a Root node enforces that there is an exposed method for sending actions to the
  // entire tree. the implementation is separated from the 'reduce' method in the Node
  // class so that it allows for some mediating transport protocol, e.g. concurrent queue.
  template<typename LowFreqAction, typename HighFreqAction>
  class Root {
  public:
    virtual ~Root() = default;

    virtual void send_action(const LowFreqAction&) = 0;
    virtual void send_action(LowFreqAction&&) = 0;
    
    virtual void send_action(const HighFreqAction&) = 0;
    virtual void send_action(HighFreqAction&&) = 0;
    
    virtual void listen() = 0;
  };

}

#endif
