#include <fstream>

#include "../EventReadFromFile.h"
#include "../Event.h"

using namespace std;

// read data from file "name"
EventReadFromFile::EventReadFromFile(const string& name) :
		file(new ifstream(name)) {
	if (!(*file)) {
		cerr << "[ERROR] File not opened" << endl;
		throw -1;
	}
}

EventReadFromFile::~EventReadFromFile() {
	delete file;
}

// get an event
const Event* EventReadFromFile::get() {
	return readFile();
}

// read an event
const Event* EventReadFromFile::readFile() {

	Event* ev;
	int event_ID;
	// try to read input file
	if (*file >> event_ID)
		ev = new Event(event_ID);
	else
		return nullptr;

	// read number of points
	unsigned numpoints;
	*file >> numpoints;

	// read and store energy loss
	int energy_loss;
	for (unsigned i = 0; i < numpoints; ++i) {
		*file >> energy_loss;
		ev->add(energy_loss);
	}

	return ev;

}

