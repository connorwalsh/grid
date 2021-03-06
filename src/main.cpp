#include <memory>
#include <iostream>

#include "anemone/io.hpp"
#include "anemone/anemone.hpp"

#include <execinfo.h>
#include <signal.h>
#include <unistd.h>


void seg_fault_handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, seg_fault_handler);

  if ( argc != 2 ) {
    std::cout << "Error: you must provide the path to the configuration file!\n";
    return -1;
  }


  Anemone anemone(argv[1],
                  std::make_shared<Monome>(),
                  std::make_shared< MidiDeviceFactoryFor<RTMidiIn, RTMidiOut> >());

  anemone.run();

  return 0;
}
