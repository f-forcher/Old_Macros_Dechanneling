#ifndef EventReadFromFile_h
#define EventReadFromFile_h

#include "EventSource.h"

#include <iostream>
#include <fstream>
#include <string>
class Event;

class EventReadFromFile: public EventSource {

public:

	// read data from file "name"
	EventReadFromFile(const std::string& name);
	virtual ~EventReadFromFile();

private:

	std::ifstream* file;

	// read and event
	const Event* readFile();

	EventReadFromFile(const EventReadFromFile& x);
	EventReadFromFile& operator=(const EventReadFromFile& x);

	// get an event
	virtual const Event* get();
};

#endif

