/*
 * cameraStatus.h
 *
 *  Created on: Feb 18, 2014
 *      Author: oystha
 */

#ifndef CAMERASTATUS_H_
#define CAMERASTATUS_H_

#include <string>
#include <mutex>

struct status_t {
	std::string mode;
	bool battery;			//true = low
	bool betamaxDV; 		//true = DV

	std::string tapeSpeed;
	std::string audio;
	bool servo;
	bool recordProtection;
	bool RCTC;				//Rewritable consumer timecode

	//DV mode 00,
	std::string DV_input;
	int DV_line;
	//DV mode 01
	std::string DV_type; 	//player or camera

	//real time counter
	//equal to tape time? -> 00:00 at TAPE TOP
	int rt_daysOnes;
	int rt_daysTens;
	int rt_hoursOnes;
	int rt_hoursTens;
	int rt_minutesOnes;
	int rt_minutesTens;
	int rt_secondsOnes;
	int rt_secondsTens;

	//memory stick
	bool ms_available;
	bool ms_inserted;
	bool ms_writeProtected;
	bool ms_play;
	bool ms_search;
	bool ms_full;

	//photo mode
	bool pm_available;
	bool pm_selected;

	//Data guide, value of _7LN when _5HN == 8
	int date_Time;

	//date and time of recorded video
	int d_yearTens;
	int d_yearOnes;
	int d_monthTens;
	int d_monthOnes;
	int d_dayTens;
	int d_dayOnes;
	int t_hoursOnes;
	int t_hoursTens;
	int t_minutesOnes;
	int t_minutesTens;
	int t_secondsOnes;
	int t_secondsTens;
};

class cameraStatus {

	std::mutex mut;

public:
	cameraStatus();
	~cameraStatus();
	status_t getStatus();
	void setStatus(status_t p);
	void setDate_Time(int value);
	bool LancToPlain(char* Buffer);
	std::string getStatusString();
	void printStatus();

private:
	status_t status;
};

//bool operator ==(const cameraStatus& status1, const cameraStatus& status2);




#endif /* CAMERASTATUS_H_ */
