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

int finish_with_error(MYSQL *con);

int initDB(MYSQL** con, std::string user, std::string password, std::string dataBase, std::string table);
int writeStatusToDB(MYSQL* con, std::string table, std::string status);

#endif /* STATUSSQL_H_ */
