#include "../EventSource.h"

#include "../Event.h"
#include "../util/include/Dispatcher.h"

EventSource::EventSource() {
}

EventSource::~EventSource() {
}

void EventSource::run() {
	const Event* ev;
	while ((ev = get())) {
		Dispatcher<Event>::notify(*ev);
		delete ev;
	}
}
