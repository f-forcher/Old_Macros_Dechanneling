#ifndef Event_h
#define Event_h

#include <vector>

class Event {

public:

	// create event with number "event_ID"
	Event(int event_ID);
	~Event();

	// function to add a point
	void add(int energyLoss);

	// get event-specific informations:
	//   event number
	//   number of points in the event
	//   energy for point "i"
	int eventNumber() const;
	unsigned int dataSize() const;
	int energy(unsigned int i) const;

	// get general informations
	static unsigned int maxSize();
	static unsigned int minSize();

	// C++11 standard syntax to prevent the compiler to automatically generate them;
	Event(const Event& x) = delete;
	Event& operator=(const Event& x) = delete;

private:

	// dummy copy constructor and assignment to prevent unadvertent copy
	// In C++11 better done through explicitly deleted functions, see above
	// Event           ( const Event& x );
	// Event& operator=( const Event& x );

	// shared variables to contain min and max number of points
	static const unsigned MIN_POINTS;
	static const unsigned MAX_POINTS;

	// event-specific informations:
	//   number of points in the event
	//   array with energies
	//...
	int event_ID_;
	std::vector<int> energies_;

};

#endif

