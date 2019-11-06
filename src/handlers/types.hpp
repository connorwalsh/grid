#ifndef HANDLER_FN_H
#define HANDLER_FN_H

#include <vector>

#include <monome.h>

#include "../io/io.hpp"
#include "../state/state.hpp"


typedef void (*midi_handler)(IO *, State*, std::vector<unsigned char> *);
typedef void (*monome_handler)(IO *, State*, const monome_event_t *);

#endif