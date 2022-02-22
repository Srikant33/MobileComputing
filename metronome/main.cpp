#include <chrono>
#include <thread>

#include<iostream>
using namespace std;
using namespace std::chrono_literals;

#include <cpprest/http_msg.h>
#include <wiringPi.h>

#include "metronome.hpp"
#include "rest.hpp"

// ** Remember to update these numbers to your personal setup. **
#define LED_RED   17
#define LED_GREEN 27
#define BTN_MODE  23
#define BTN_TAP   24
#define BTN_T2    18
#define BTN_T1    25


double_t minTime=DBL_MAX;
double_t maxTime=0;
double_t newTime= 1.00;
unsigned long timeToSleep = static_cast<unsigned long>( newTime * 1000 );





/*

void start_timing(){
		start = std::chrono::system_clock::now();
		avg=0;
		countclicks=0;
		wiringPiISR(BTN_TAP, INT_EDGE_RISING, tap);
	}
	
	
	// Call when leaving "learn" mode
double_t stop_timing(){
	if (countclicks>3){
		newTime=avg;
		cout<<"\n\n\n final avg = "<< avg;
		
		
		return avg;
	} else{
		return 0;
	}
}

*/























// Mark as volatile to ensure it works multi-threaded.
volatile bool btn_mode_pressed = false;

// Run an additional loop separate from the main one.
void blink() {
	bool on = false;
	// ** This loop manages LED blinking. **
	while (true) {
		
		timeToSleep = static_cast<unsigned long>( newTime * 1000 );
		//cout<<timeToSleep;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
		//int waitTime= (int)(newTime*10000000);
		// The LED state will toggle once every second.
		//cout<<waitTime;
		//std::this_thread::sleep_for(std::chrono::microseconds(waitTime));
		//std::this_thread::sleep_for(1s);

		// Perform the blink if we are pressed,
		// otherwise just set it to off.
		if (btn_mode_pressed)
			on = !on;
		else
			on = false;

		// HIGH/LOW probably equal 1/0, but be explicit anyways.
		digitalWrite(LED_GREEN, (on) ? HIGH : LOW);
	}
}

// This is called when a GET request is made to "/answer".
void get42(web::http::http_request msg) {
	msg.reply(200, web::json::value::number(42));
}




//
void changeMode(){
	metronome met;
	cout<<"Button 1\n";
	
	//btn_mode_pressed = (digitalRead(BTN_MODE));

	if(btn_mode_pressed == TRUE){
		btn_mode_pressed = FALSE;
		met.start_timing();
		//wiringPiISR(BTN_TAP, INT_EDGE_RISING, learnMode);
		
		
		//cout<<"clicked off";

	} else{
		double_t retTime;
		
		retTime = met.stop_timing();
		
		if (retTime > 0.005){
			::newTime = retTime;
			cout<<"\n  New Time"<<::newTime;
			if (newTime < ::minTime){
			::minTime=newTime;
			}
			if (newTime > ::maxTime){
				::maxTime=newTime;
			}
		} 
		//cout<<"\n\n\n\n newtime now";
		
		cout<<"\n Maximum Interval till now "<<::maxTime;
		cout<<"\n Minimum Interval till now "<<::minTime;
		btn_mode_pressed = TRUE;
		//cout<<"clicked on";

	}
	//cout<<"clicked once";
	//btn_mode_pressed = TRUE;
}

/*
void learnMode(){
	//btn_mode_pressed = (digitalRead(BTN_MODE));
	cout<<"\n clicked once";
	//btn_mode_pressed = TRUE;
}
*/
 
// This program shows an example of input/output with GPIO, along with
// a dummy REST service.
// ** You will have to replace this with your metronome logic, but the
//    structure of this program is very relevant to what you need. **


int main() {
	// WiringPi must be initialized at the very start.
	// This setup method uses the Broadcom pin numbers. These are the
	// larger numbers like 17, 24, etc, not the 0-16 virtual ones.
	wiringPiSetupGpio();

	// Set up the directions of the pins.
	// Be careful here, an input pin set as an output could burn out.
	pinMode(LED_RED, OUTPUT);
	pinMode(BTN_MODE, INPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(BTN_T1, OUTPUT);
	pinMode(BTN_T2, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);



	
	// Note that you can also set a pull-down here for the button,
	// if you do not want to use the physical resistor.
	//pullUpDnControl(BTN_MODE, PUD_DOWN);

	// Configure the rest services after setting up the pins,
	// but before we start using them.
	// ** You should replace these with the BPM endpoints. **
	auto get42_rest = rest::make_endpoint("/answer");
	get42_rest.support(web::http::methods::GET, get42);

	// Start the endpoints in sequence.
	get42_rest.open().wait();
	
	// Use a separate thread for the blinking.
	// This way we do not have to worry about any delays
	// caused by polling the button state / etc.
	std::thread blink_thread(blink);
	blink_thread.detach();
	
	
	
	//
	wiringPiISR(BTN_MODE, INT_EDGE_RISING, changeMode);
	
	
	int a=0;
	// ** This loop manages reading button state. **
	while (true) {
		// We are using a pull-down, so pressed = HIGH.
		// If you used a pull-up, compare with LOW instead.
		
		// btn_mode_pressed = FALSE;
		/*
		 * 
		
		btn_mode_pressed = (digitalRead(BTN_MODE));
		
		
		
		if(btn_mode_pressed == TRUE){
			btn_mode_pressed = FALSE;
		} else{
			btn_mode_pressed = FALSE;
		}
		*/
		a=0;
		
		cin>>a;
		if (a==1){
			//cout<<" one pressed/n";
			digitalWrite(BTN_T1, HIGH);
			std::this_thread::sleep_for(100ms);
			digitalWrite(BTN_T1, LOW);
		} else if(a==2){
			//cout<<" two pressed/n";
			digitalWrite(BTN_T2, HIGH);
			std::this_thread::sleep_for(100ms);
			digitalWrite(BTN_T2, LOW);
		} else {
			continue;
		}
			
		
		
		// btn_mode_pressed = (digitalRead(BTN_MODE) == HIGH);
		// Delay a little bit so we do not poll too heavily.
		std::this_thread::sleep_for(10ms);

		// ** Note that the above is for testing when the button
		// is held down. For detecting "taps", or momentary
		// pushes, you need to check for a "rising edge" -
		// this is when the button changes from LOW to HIGH... **
	}

	return 0;
}
