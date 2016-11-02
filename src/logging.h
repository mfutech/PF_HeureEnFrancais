/*
 * logging.h
 *
 *  Created on: 29 oct. 2016
 *      Author: Marc
 */

#pragma once

#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_

#define DEBUG

#ifdef DEBUG
#define MY_APP_LOG(level, fmt, args...)                                \
		app_log(level, __FILE_NAME__, __LINE__, fmt, ## args)
#else
#define MY_APP_LOG(level, fmt, args...)
#endif


#endif /* SRC_LOGGING_H_ */
