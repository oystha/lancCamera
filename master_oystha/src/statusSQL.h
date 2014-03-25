/*
 * statusSQL.h
 *
 *  Created on: Mar 13, 2014
 *      Author: oystha
 */

#ifndef STATUSSQL_H_
#define STATUSSQL_H_

#include <string>

#include <mysql.h>

#include "div.h"

int finish_with_error(MYSQL *con);

int initDB(MYSQL** con, SQL_options_t* SQL);
int writeStatusToDB(MYSQL* con, std::string table, std::string status);

#endif /* STATUSSQL_H_ */
