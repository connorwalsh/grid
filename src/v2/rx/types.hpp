#ifndef RX_TYPES_H
#define RX_TYPES_H

#include <memory>


namespace rx {

  namespace dag {
    template<typename T, typename A>
    class State;  // forward declaration

    namespace high_frequency {
      template<typename T, typename A>
      class State;  // forward declaration      
    }
  }
  
  namespace types {
    
    template<typename T, typename A>
    using state_ptr = std::shared_ptr<dag::State<T, A> >;

    namespace high_frequency {
      template<typename T, typename A>
      using state_ptr = std::shared_ptr<dag::high_frequency::State<T, A> >;
    }
  }
}

#endif
