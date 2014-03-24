/*
 * cameraStatus.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: oystha
 */

#include <iostream>
#include <string>
#include <sstream>
#include <mutex>

#include "cameraStatus.h"
#include "div.h"

using namespace std;



cameraStatus::cameraStatus() {
	status.mode 				= "";
	status.battery				= false;
	status.betamaxDV 			= true;
	status.tapeSpeed 			= 0b00;
	status.audio 				= false;
	status.servo 				= false;
	status.recordProtection 	= false;
	status.RCTC					= false;

	//DV
	status.DV_input 			= "";
	status.DV_line 				= 0b0000;
	status.DV_type 				= "";

	//real time counter
	status.rt_daysOnes 			= 0b00;
	status.rt_daysTens 			= 0b00;
	status.rt_hoursOnes 		= 0b00;
	status.rt_hoursTens 		= 0b00;
	status.rt_minutesOnes 		= 0b00;
	status.rt_minutesTens 		= 0b00;
	status.rt_secondsOnes 		= 0b00;
	status.rt_secondsTens 		= 0b00;

	//memory stick
	status.ms_available			= false;
	status.ms_inserted			= false;
	status.ms_writeProtected	= false;
	status.ms_play				= false;
	status.ms_search			= false;
	status.ms_full				= false;

	//photo mode
	status.pm_available			= false;
	status.pm_selected			= false;

	//date and time
	status.date_Time			= 0;
	status.d_yearTens			= 0b00;
	status.d_yearOnes			= 0b00;
	status.d_monthTens			= 0b00;
	status.d_monthOnes			= 0b00;
	status.d_dayTens			= 0b00;
	status.d_dayOnes			= 0b00;
	status.t_hoursOnes 			= 0b00;
	status.t_hoursTens 			= 0b00;
	status.t_minutesOnes 		= 0b00;
	status.t_minutesTens 		= 0b00;
	status.t_secondsOnes 		= 0b00;
	status.t_secondsTens 		= 0b00;
}

cameraStatus::~cameraStatus() {}

void cameraStatus::setStatus(status_t p){
	mut.lock();
	status = p;
	/*
	mode 				= p->mode;
	betamaxDV 			= p->betamaxDV;
	battery				= p->battery;
	tapeSpeed 			= p->tapeSpeed;
	audio 				= p->audio;
	servo 				= p->servo;
	recordProtection 	= p->recordProtection;
	RCTC				= p->RCTC;

	//DV
	DV_input 			= p->DV_input;
	DV_line 			= p->DV_line;
	DV_type				= p->DV_type;

	//real time counter
	rt_daysOnes 		= p->rt_daysOnes;
	rt_daysTens 		= p->rt_daysTens;
	rt_hoursOnes 		= p->rt_hoursOnes;
	rt_hoursTens 		= p->rt_hoursTens;
	rt_minutesOnes 		= p->rt_minutesOnes;
	rt_minutesTens 		= p->rt_minutesTens;
	rt_secondsOnes 		= p->rt_secondsOnes;
	rt_secondsTens 		= p->rt_secondsTens;

	//memory stick
	ms_available		= p->ms_available;
	ms_inserted			= p->ms_inserted;
	ms_writeProtected	= p->ms_writeProtected;
	ms_play				= p->ms_play;
	ms_search			= p->ms_search;
	ms_full				= p->ms_full;

	//photo mode
	pm_available		= p->pm_available;
	pm_selected			= p->pm_selected;
	*/

	mut.unlock();
}

status_t cameraStatus::getStatus() {
	status_t p;
	mut.lock();
	p = this->status;
	mut.unlock();
	return p;
}

void cameraStatus::setDate_Time(int value) {
	mut.lock();
	status.date_Time = value;
	mut.unlock();
}

string cameraStatus::getStatusString(){
	status_t copy = getStatus();
	stringstream ss;
	ss << "'" 	<< copy.mode				<< "',";
	ss 			<< copy.battery 			<< ",";
	ss 			<< copy.betamaxDV			<< ",";
	ss			<< copy.tapeSpeed			<< ",";
	ss			<< copy.audio				<< ",";
	ss			<< copy.recordProtection	<< ",";
	ss			<< copy.RCTC				<< ",";
	ss << "'"	<< copy.DV_input			<< "',";
	ss			<< copy.DV_line				<< ",";
	ss << "'"	<< copy.DV_type				<< "',";
	ss 			<< copy.rt_daysTens			<< ",";
	ss 			<< copy.rt_daysOnes			<< ",";
	ss 			<< copy.rt_hoursTens		<< ",";
	ss 			<< copy.rt_hoursOnes		<< ",";
	ss 			<< copy.rt_minutesTens		<< ",";
	ss 			<< copy.rt_minutesOnes		<< ",";
	ss 			<< copy.rt_secondsTens		<< ",";
	ss 			<< copy.rt_secondsOnes		<< ",";
	ss			<< copy.d_yearTens			<< ",";
	ss			<< copy.d_yearOnes			<< ",";
	ss			<< copy.d_monthTens			<< ",";
	ss			<< copy.d_monthOnes			<< ",";
	ss			<< copy.d_dayTens			<< ",";
	ss			<< copy.d_dayOnes			<< ",";
	ss			<< copy.t_hoursTens			<< ",";
	ss			<< copy.t_hoursOnes			<< ",";
	ss			<< copy.t_minutesTens		<< ",";
	ss			<< copy.t_minutesOnes		<< ",";
	ss			<< copy.t_secondsTens		<< ",";
	ss			<< copy.t_secondsTens		<< ",";
	ss			<< copy.ms_available		<< ",";
	ss			<< copy.ms_inserted			<< ",";
	ss			<< copy.ms_writeProtected	<< ",";
	ss			<< copy.ms_play				<< ",";
	ss			<< copy.ms_search			<< ",";
	ss			<< copy.ms_full				<< ",";
	ss			<< copy.pm_available		<< ",";
	ss			<< copy.pm_selected;
	return ss.str();
}


/*
 * Return true if status has been updated.
 */
bool cameraStatus::LancToPlain(char* Buffer){
	status_t copy = getStatus();
	char _5LN = charToByte(Buffer[3]);
	char _6HN = charToByte(Buffer[4]);
	char _6LN = charToByte(Buffer[5]);
	char _7HN = charToByte(Buffer[6]);
	char _7LN = charToByte(Buffer[7]);
	bool update = false;
	string mode 	= copy.mode;
	string input 	= copy.DV_input;
	string type 	= copy.DV_type;
	int line 		= copy.DV_line;
	switch(Buffer[0]){
	case '0':
		switch(Buffer[1]){
		case '0':
			//initial
			mode = "INITIAL";
			break;
		case '1':
			//is eject
			mode = "IS EJECT";
			break;
		case '2':
			//stop
			mode = "STOP";
			break;
		case '3':
			//fwd
			mode = "FWD";
			break;
		case '4':
			//rec
			mode = "REC";
			break;
		case '6':
			//play
			mode = "PLAY";
			break;
		case '7':
			//play/pause fwd
			mode = "PLAY/PAUSE FWD";
			break;
		case '8':
			//AL insert
			mode = "AL INSERT";
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '1':
		switch(Buffer[1]){
		case '1':
			//dew cass. out
			mode = "DEW CASS: OUT";
			break;
		case '2':
			//load
			mode = "LOAD";
			break;
		case '4':
			//rec/pause
			mode = "REC/PAUSE";
			break;
		case '7':
			//frame fwd
			mode = "FRAME FWD";
			break;
		case '8':
			//AL ins-pause
			mode = "AL INS_PAUSE";
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '2':
		switch(Buffer[1]){
		case '1':
			//ejecting
			break;
		case '2':
			//cassett busy
			break;
		case '4':
			//timer-rec
			break;
		case '6':
			//x1 fwd
			break;
		case '7':
			//1/5 fwd
			break;
		case '8':
			//AR insert
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '3':
		switch(Buffer[1]){
		case '1':
			//unload
			break;
		case '2':
			//low-battery
			mode = "LOW BATTERY";
			break;
		case '3':
			//go zero/play f.
			break;
		case '4':
			//timer-rec s.
			break;
		case '6':
			//x1 rev
			break;
		case '7':
			//1/5 rev
			break;
		case '8':
			//AR ins-pauselanc
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '4':
		switch(Buffer[1]){
		case '2':
			//dew stop
			break;
		case '3':
			//fwd mem stop
			break;
		case '4':
			//AV insert
			break;
		case '6':
			//cue
			break;
		case '7':
			//1/10 fwd
			break;
		case '8':
			//AL+V insert
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '5':
		switch(Buffer[1]){
		case '2':
			//emergency
			break;
		case '4':
			//AV ins.-pause
			break;
		case '6':
			//rev
			break;
		case '7':
			//1/10 rev
			break;
		case '8':
			//AL+V ins-ps
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '6':
		switch(Buffer[1]){
		case '2':
			//tape end
			mode = "TAPE END";
			break;
		case '4':
			//video insert
			break;
		case '6':
			//x2/x3 fwd
			break;
		case '7':
			//frame fwd
			break;
		case '8':
			//AR+V insert
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '7':
		switch(Buffer[1]){
		case '2':
			//tape top
			mode = "TAPE TOP";
			break;
		case '4':
			//video ins-ps
			break;
		case '6':
			//x2/x3 rev
			break;
		case '7':
			//frame rev
			break;
		case '8':
			//AL+R ins-ps
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '8':
		switch(Buffer[1]){
		case '3':
			//rew
			mode = "REW";
			break;
		case '4':
			//audio dub
			break;
		case '5':
			//edit search+
			break;
		case '6':
			//x9 fwd
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '9':
		switch(Buffer[1]){
		case '2':
			//stp after zero
			break;
		case '4':
			//a.dub pause
			break;
		case '5':
			//edit search
			break;
		case '6':
			//x9 rev
			break;
		case '7':
			//play/pause rev
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'A':
		switch(Buffer[1]){
		case '2':
			//load emer.
			break;
		case '3':
			//auto-play
			break;
		case '4':
			//cam rec
			break;
		case '5':
			//edit-s fwd
			break;
		case '6':
			//frame sea. cue
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'B':
		switch(Buffer[1]){
		case '1':
			//unload emerg.
			break;
		case '2':
			//stop emerg. 1
			break;
		case '3':
			//go zero/play r.
			break;
		case '4':
			//cam stby
			break;
		case '5':
			//edit-s rev
			break;
		case '6':
			//frame sea rev.
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'C':
		switch(Buffer[1]){
		case '2':
			//stop emerg. 2
			break;
		case '3':
			//rew mem stop
			break;
		case '6':
			//x14 fwd
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'D':
		switch(Buffer[1]){
		case '3':
			//hi-speed rew
			break;
		case '6':
			//x14 rev
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'E':
		switch(Buffer[1]){
		case '2':
			//stop NC
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'F':
		switch(Buffer[1]){
		case '5':
			//edit pause
			break;
		default:
			//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	default:
		//cout << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
		break;
	}
	if(copy.mode != mode){
		update = true;
		copy.mode = mode;
	}
	switch(Buffer[2]){
	case '1':
		//Status V8 and Hi8
		if(_6LN & 0b0000){
			//tape speed SP (Standard Play)
		}
		if((_6LN & 0b0001) && !(_6LN & 0b0010)){
			//tape speed LP (Long Play)
		}
		if(_6LN & 0b0100){
			//standard FM-sound
		}
		if(_6LN & 0b1000){
			//PCM-sound
		}
		if(_6HN & 0b0001){
			//camera-mode
		}
		if(_6HN & 0b0010){
			//rec protection / tape pre-end
		}
		if(_6HN & 0b0100){
			//ME
		} else {
			//MP
		}
		if(_6HN & 0b1000){
			//13 micro-m
		} else {
			//10 micro-m
		}
		if(_7LN & 0b0010){
			//lanc-mode
		}
		if(_7LN & 0b1000){
			//servo / mechanics on
		}
		if(_7HN & 0b0101){
			//line
		} else {
			//tuner
		}
		if((_7HN & 0b0100) && !(_7HN & 0b0001)){
			//sim
		}
		break;
	case '2':
		//decimal counter
		//_6LN 	One's
		//_6HN	Ten's
		//_7LN 	Hundred's
		//_7HN 	Thousand's
		//cout << "Decimal counter " << _7HN << " " << _7LN << " " << _6HN << " " << _6LN << ", ";
		break;
	case '3':
		//real time counter
		//picture counter
		//_6LN 	Seconds One's / Picture One's
		//_6HN	Seconds Ten's / Picture Ten's
		//_7LN 	Minutes One's / Picture Hundred's
		//_7HN 	Minutes Ten's / Picture Thousand's
		//cout << "Real time counter (MM:SS) " << _7HN << " " << _7LN << ":" <<_6HN << " " << _6LN << ", ";
		if(copy.rt_minutesTens != _7HN || copy.rt_minutesOnes != _7LN){
			update = true;
			copy.rt_minutesTens = _7HN;
			copy.rt_minutesOnes = _7LN;
		}
		if(copy.rt_secondsTens != _6HN || copy.rt_secondsOnes != _6LN){
			update = true;
			copy.rt_secondsTens = _6HN;
			copy.rt_secondsOnes = _6LN;
		}
		break;
	case '4':
		//real time counter
		//_6LN 	Hours One's
		//_6HN	Hours Ten's
		//_7LN	Days One's
		//_7HN 0+1	Days Ten's
		//_7HN 2	RCTC (Rewritable consumer timecode)
		//_7HN 3	sign(1=negativ)(or 0000/1111)?
		if(copy.RCTC != (bool)(_7HN & 0b0100)){
			update = true;
			copy.RCTC = (bool)(_7HN & 0b0100);
			//RCTC (Rewritable consumer timecode)
			//cout << "RCTC, ";
		}
		//byte 7 is probably not days, changes rapidly...
		//if(copy.rt_daysTens != (_7HN & 0b0011) || copy.rt_daysOnes != _7LN){
		//	update = true;
		//	copy.rt_daysTens = (_7HN & 0b0011);
		//	copy.rt_daysOnes = _7LN;
		//}
		if(copy.rt_hoursTens != _6HN || copy.rt_hoursOnes != _6LN){
			update = true;
			copy.rt_hoursTens = _6HN;
			copy.rt_hoursOnes = _6LN;
		}
		break;
	case '5':
		//remain time
		//_6LN 	Minutes One's
		//_6HN	Minutes Ten's
		//_7LN 	Hours One's
		//cout << "Remain time " << _7LN << ":" << _6HN << " " << _6LN << ", ";
		if(!_7HN & 0b0100){
			//calculating
		}
		break;
	case '7':
		//status Betamax or DV
		if(copy.betamaxDV != (bool)(_6HN & 0b0001)){
			update = true;
			copy.betamaxDV = (bool)(_6HN & 0b0001);
		}
		if(copy.recordProtection != (bool)(_6HN & 0b0010)){
			update = true;
			copy.recordProtection = (bool)(_6HN & 0b0010);
		}
		//_6HN 2+3 Byte 7 DV mode
		//cout << "mode " << (_6HN >> 2) << ", ";
		switch(_6HN >> 2){
		//DV mode
		case 0b00:
			switch(_7LN){
			case 0b0000:
				//input tuner
				input = "TUNER";
				break;
			case 0b0101:
				//input line
				input = "LINE";
				break;
			case 0b1010:
				//input DV
				input = "DV";
				break;
			}
			switch(_7HN){
			case 0b0000:
				//Line 1
				line = 1;
				break;
			case 0b0101:
				//Line 2
				line = 2;
				break;
			case 0b1010:
				//Line 3
				line = 3;
				break;
			case 0b1111:
				//Line 4
				line = 4;
				break;
			}
			break;
		case 0b01:
			switch(_7LN){
			case 0b0000:
				//player
				type = "PLAYER";
				break;
			case 0b0011:
				//camera
				type = "CAMERA";
				break;
			}
			break;
		default:
			//cout << "UNKOWN! ";
			break;
		}
		if(copy.DV_input != input){
			update = true;
			copy.DV_input = input;
		}
		if(copy.DV_line != line){
			update = true;
			copy.DV_line = line;
		}
		if(copy.DV_type != type){
			update = true;
			copy.DV_type = type;
		}
		if(copy.tapeSpeed != (bool)(_6LN & 0b0011)){
			update = true;
			copy.tapeSpeed = (bool)(_6LN & 0b0011);
		}
		if(copy.audio != (bool)(_6LN & 0b0100)){
			update = true;
			copy.audio = (bool)(_6LN & 0b0100);
		}
		break;
	case '8':
		//Data Guide
		//_6LN 4
		//_6HN 1
		//_7LN 3=Date, 4=Time
		this->setDate_Time(_7LN);
		//_7HN 0
		break;
	case '9':
		//Data Code
		//(Tape + Picture)
		if(copy.date_Time == 3) {
			if(copy.d_monthTens != _7HN || copy.d_monthOnes != _7LN){
				update = true;
				copy.d_monthTens = _7HN;
				copy.d_monthOnes = _7LN;
			}
			if(copy.d_yearTens != _6HN || copy.d_yearOnes != _6LN){
				update = true;
				copy.d_yearTens = _6HN;
				copy.d_yearOnes = _6LN;
			}
		} else if(copy.date_Time == 4) {
			if(copy.t_minutesTens != _7HN || copy.t_minutesOnes != _7LN){
				update = true;
				copy.t_minutesTens = _7HN;
				copy.t_minutesOnes = _7LN;
			}
			if(copy.t_hoursTens != _6HN || copy.t_hoursOnes != _6LN){
				update = true;
				copy.t_hoursTens = _6HN;
				copy.t_hoursOnes = _6LN;
			}
		}
		break;
	case 'A':
		//Data Code
		//(Tape + Picture)
		if(copy.date_Time == 3) {
			if(copy.d_dayTens != _6HN || copy.d_dayOnes != _6LN){
				update = true;
				copy.d_dayTens = _6HN;
				copy.d_dayOnes = _6LN;
			}
		} else if(copy.date_Time == 4) {
			if(copy.t_secondsTens != _6HN || copy.t_secondsOnes != _6LN){
				update = true;
				copy.t_secondsTens = _6HN;
				copy.t_secondsOnes = _6LN;
			}
		}
		break;
	case 'B':
		//Status AVCHD
		if(_6LN & 0b0001){
			//AVCHD lanc mode
		}
		if(_6LN & 0b0010){
			//AVCHD photo mode
		}
		break;
	case 'C':
		//Status Hi8, DV (+GC1)
		break;
	case 'E':
		//Status Photo and Memory Stick
		//photo mode selected
		if(copy.pm_selected != (bool)(_6LN & 0b0001)){
			update = true;
			copy.pm_selected = (bool)(_6LN & 0b0001);
		}
		//memory stick inserted
		if(copy.ms_inserted != (bool)(_6LN & 0b0010)){
			update = true;
			copy.ms_inserted = (bool)(_6LN & 0b0010);
		}
		//memory stick write-protected
		if(copy.ms_writeProtected != (bool)(_6LN & 0b0100)){
			update = true;
			copy.ms_writeProtected = (bool)(_6LN &0b0100);
		}
		//memory stick play
		if(copy.ms_play != (bool)(_6HN & 0b0001)){
			update = true;
			copy.ms_play = (bool)(_6HN & 0b0001);
		}
		//memory stick search
		if(copy.ms_search != (bool)(_6HN & 0b0010)){
			update = true;
			copy.ms_search = (bool)(_6HN & 0b0010);
		}
		//memory stick full
		if(copy.ms_full != (bool)(_6HN & 0b0100)){
			update = true;
			copy.ms_full = (bool)(_6HN & 0b0100);
		}
		//camera with memory stick
		if(copy.ms_available != (bool)(_6HN & 0b1000)){
			update = true;
			copy.ms_available = (bool)(_6HN & 0b1000);
		}
		//photo mode available
		if(copy.pm_available != (bool)(_7LN & 0b0001)){
			update = true;
			copy.pm_available = (bool)(_7LN & 0b0001);
		}
		break;
	default:
		//cout << "Unknown value for Byte 2: " << Buffer[2];
		break;
	}
	if(_5LN & 0b0001){
		//invalid transmitted code
		//cout << "INVALID TRANSMITTED CODE, ";
	}
	//rec protection
	if(copy.recordProtection != (bool)(_5LN & 0b0010)){
		update = true;
		copy.recordProtection = (bool)(_5LN & 0b0010);
	}
	//battery low
	if(copy.battery != (bool)(_5LN & 0b0100)){
		update = true;
		copy.battery = (bool)(_5LN & 0b0100);
	}
	if(_5LN & 0b1000){
		//zero mem / zero found
		//cout << "ZERO MEM / ZERO FOUND";
	}
	//cout << endl;
	if(update){
		setStatus(copy);
		//cout << "Buffer:" << Buffer;
		//printStatus();
	}
	return update;
}

void cameraStatus::printStatus(){
	status_t copy = getStatus();
	cout << "Mode: " << copy.mode << endl;
	cout << " Real Time (HH:MM:SS): " /*<< copy.rt_daysTens << copy.rt_daysOnes << ":"*/ << copy.rt_hoursTens << copy.rt_hoursOnes << ":" << copy.rt_minutesTens << copy.rt_minutesOnes <<":" << copy.rt_secondsTens << copy.rt_secondsOnes << endl;
	cout << " Date      (YY:MM:DD): " << copy.d_yearTens << copy.d_yearOnes << ":" << copy.d_monthTens << copy.d_monthOnes << ":" << copy.d_dayTens << copy.d_dayOnes << endl;
	cout << " Time      (HH:MM:SS): " << copy.t_hoursTens << copy.t_hoursOnes << ":" << copy.t_minutesTens << copy.t_minutesOnes << ":" << copy.t_secondsTens << copy.t_secondsOnes << endl;
	if(copy.betamaxDV){
		cout << "DV, input: " << copy.DV_input << ", line: " << copy.DV_line << ", type: " << copy.DV_type << endl;
	} else {
		cout << "Betamax" << endl;
	}
	if(copy.battery){
		cout << "Battery: LOW" << endl;
	}
	cout << "Audio: " << ((copy.audio)? "16bit, ":"12bit, ");
	cout << "Tape Speed: " << copy.tapeSpeed << ", Servo: " << copy.servo << ", ";
	cout << "Record Protection: " << copy.recordProtection << ", RCTC: " << copy.RCTC << endl;
	cout << "Memory stick, available: " << copy.ms_available << ", inserted: " << copy.ms_inserted << ", write protected: " << copy.ms_writeProtected;
	cout << ", full: " << copy.ms_full << ", play: " << copy.ms_play << ", search: " << copy.ms_search << endl;
	cout << "Photo Mode, available: " << copy.pm_available << ", selected: " << copy.pm_selected << endl;
}

/*

bool operator ==(const cameraStatus& status1, const cameraStatus& status2) {
	return ((status1.mode == status2.mode)
			&& (status1.betamaxDV == status2.betamaxDV)
			&& (status1.tapeSpeed == status2.tapeSpeed)
			&& (status1.audio  == status2.audio)
			&& (status1.servo == status2.servo)
			&& (status1.recordProtection == status2.recordProtection)
			&& (status1.RCTC == status2.RCTC)
			&& (status1.DV_input == status2.DV_input)
			&& (status1.DV_line == status2.DV_line)
			&& (status1.DV_type == status2.DV_type)
			&& (status1.rt_daysOnes == status2.rt_daysOnes)
			&& (status1.rt_daysTens == status2.rt_daysTens)
			&& (status1.rt_hoursOnes == status2.rt_hoursOnes)
			&& (status1.rt_hoursTens == status2.rt_hoursTens)
			&& (status1.rt_minutesOnes == status2.rt_minutesOnes)
			&& (status1.rt_minutesTens == status2.rt_minutesTens)
			&& (status1.rt_secondsOnes == status2.rt_secondsOnes)
			&& (status1.rt_secondsTens == status2.rt_secondsTens)
			&& (status1.ms_available == status2.ms_available)
			&& (status1.ms_inserted == status2.ms_inserted)
			&& (status1.ms_writeProtected == status2.ms_writeProtected)
			&& (status1.ms_play == status2.ms_play)
			&& (status1.ms_search == status2.ms_search)
			&& (status1.ms_full == status2.ms_full)
			&& (status1.pm_available == status2.pm_available)
			&& (status1.pm_selected == status2.pm_selected));
}

*/
