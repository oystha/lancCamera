/*
 * statusSQL.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: oystha
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>


#include <mysql.h>
#include <my_global.h>


#include "statusSQL.h"

using namespace std;

int finish_with_error(MYSQL *con){
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	return -1;
}

int initDB(MYSQL **con, string user, string password, string dataBase, string table){
	string query;
	*con = mysql_init(NULL);
	if(*con == NULL){
		return finish_with_error(*con);
	}
	if(mysql_real_connect(*con, "localhost", user.c_str(), password.c_str(), dataBase.c_str(), 0, NULL, 0) == NULL){
		query = "CREATE DATABASE " + dataBase;
		if(mysql_query(*con, query.c_str())) {
			return finish_with_error(*con);
		}
	}
	query = "DROP TABLE IF EXISTS " + table;
	if(mysql_query(*con, query.c_str())){
		return finish_with_error(*con);
	}
	stringstream ss;
	ss << "CREATE TABLE ";
	ss << table;
	ss <<   " ("
			"ID int NOT NULL AUTO_INCREMENT,"
			"timestamp TIMESTAMP,"
			"mode TINYTEXT,"
			"battery TINYINT,"
			"tapeSpeed TINYINT,"
			"audio TINYINT,"
			"servo TINYINT,"
			"recordProtection TINYINT,"
			"RCTC TINYINT,"
			"DV_input TINYTEXT,"
			"DV_line TINYTEXT,"
			"DV_type TINYTEXT,"
			"rt_daysTens TINYINT,"
			"rt_daysOnes TINYINT,"
			"rt_hoursTens TINYINT,"
			"rt_hoursOnes TINYINT,"
			"rt_minutesTens TINYINT,"
			"rt_minutesOnes TINYINT,"
			"rt_secondsTens TINYINT,"
			"rt_secondsOnes TINYINT,"
			"d_yearTens TINYINT,"
			"d_yearOnes TINYINT,"
			"d_monthTens TINYINT,"
			"d_monthOnes TINYINT,"
			"d_dayTens TINYINT,"
			"d_dayOnes TINYINT,"
			"t_hoursTens TINYINT,"
			"t_hoursOnes TINYINT,"
			"t_minutesTens TINYINT,"
			"t_minutesOnes TINYINT,"
			"t_secondsTens TINYINT,"
			"t_secondsOnes TINYINT,"
			"ms_available TINYINT,"
			"ms_inserted TINYINT,"
			"ms_writeProtected TINYINT,"
			"ms_play TINYINT,"
			"ms_search TINYINT,"
			"ms_full TINYINT,"
			"pm_available TINYINT,"
			"pm_selected TINYINT,"
			"PRIMARY KEY (ID))";
	if(mysql_query(*con,ss.str().c_str()))
	{
		return finish_with_error(*con);
	}
	/*
	if(mysql_query(*con,"INSERT INTO Camera (mode) VALUES ('TEST')"))
	{
		return finish_with_error(*con);
	}
	*/
	return 0;
}

/*
 * mysql_query(con, INSERT INTO 'table' (..) VALUES('status'))
 */
int writeStatusToDB(MYSQL *con, string table, string status){
	stringstream ss;
	ss << "INSERT INTO ";
	ss << table;
	ss <<   " ("
			"timestamp,"
			"mode,"
			"battery,"
			"tapeSpeed,"
			"audio,"
			"servo,"
			"recordProtection,"
			"RCTC,"
			"DV_input,"
			"DV_line,"
			"DV_type,"
			"rt_daysTens,"
			"rt_daysOnes,"
			"rt_hoursTens,"
			"rt_hoursOnes,"
			"rt_minutesTens,"
			"rt_minutesOnes,"
			"rt_secondsTens,"
			"rt_secondsOnes,"
			"d_yearTens,"
			"d_yearOnes,"
			"d_monthTens,"
			"d_monthOnes,"
			"d_dayTens,"
			"d_dayOnes,"
			"t_hoursTens,"
			"t_hoursOnes,"
			"t_minutesTens,"
			"t_minutesOnes,"
			"t_secondsTens,"
			"t_secondsOnes,"
			"ms_available,"
			"ms_inserted,"
			"ms_writeProtected,"
			"ms_play, ms_search,"
			"ms_full,"
			"pm_available,"
			"pm_selected"
			") ";
	ss << "VALUES(";
	ss << "NOW(),";
	ss << status;
	ss << ")";
	status = ss.str();
	//cout << "status: " << status << endl;
	if(mysql_query(con, status.c_str())){
		return finish_with_error(con);
	}
	return 0;
}




