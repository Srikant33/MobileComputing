#pragma once
#define BTN_TAP   24

#define LED_RED   17


#include <cstddef>
#include <cfloat>


std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1, 1000000000> > > start = std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1, 1000000000> > > endt = std::chrono::system_clock::now();
double_t avg=0;
int countclicks=0;

//double_t newTime= 1;

void tap(){	
		pinMode(LED_RED, OUTPUT);
		cout<<"Button 2 \n";
		digitalWrite(LED_RED, HIGH);
		endt = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = endt-start;
		start = std::chrono::system_clock::now();
		countclicks++;
		//cout<<"\n elapsed s " << elapsed_seconds.count();
		avg= (avg*(countclicks-1)+elapsed_seconds.count())/countclicks;
		//cout<<"\n new avg " << avg;
		std::this_thread::sleep_for(0.05s);
		digitalWrite(LED_RED, LOW);
}




class metronome
{
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1, 1000000000> > > start = std::chrono::system_clock::now();
		
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1, 1000000000> > > end = std::chrono::system_clock::now();
		
public:
	enum { beat_samples = 3 };

public:
	metronome()
	: m_timing(false), m_beat_count(0) {}
	~metronome() {}

public:

	// Call when entering "learn" mode
	/*
	void tap(){
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		start = std::chrono::system_clock::now();
		count++;
		avg= (avg*(count-1)+elapsed_seconds.count())/count;
	}
	*/
	
	void start_timing(){
		//cout<<"seatr timer";
		::start = std::chrono::system_clock::now();
		::avg=0;
		::countclicks=0;
		
		wiringPiISR(BTN_TAP, INT_EDGE_RISING, tap);
	}
	
	
	// Call when leaving "learn" mode
	double_t stop_timing(){
		if (countclicks>3){
			cout<<"\n  Final avg = "<< ::avg;
			return ::avg;
		} else{
			return 0;
		}
	}

	// Should only record the current time when timing
	// Insert the time at the next free position of m_beats
	
	

	bool is_timing() const { return m_timing; }
	// Calculate the BPM from the deltas between m_beats
	// Return 0 if there are not enough samples
	size_t get_bpm() const;
	
	double avg;
	int count;

private:
	bool m_timing;

	// Insert new samples at the end of the array, removing the oldest
	size_t m_beats[beat_samples];
	size_t m_beat_count;
	
	
	//void start_timing()
	
	// void tap()
	//double_t stop_timing()
};

