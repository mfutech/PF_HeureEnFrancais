/*
 * settings.h
 *
 *  Created on: 29 oct. 2016
 *      Author: Marc
 */

#pragma once
#include <pebble.h>

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_


#define KEY_MODE_ROUNDED 1
#define DEFAULT_MODE_ROUNDED 0
#define KEY_MODE_NATURAL 3
#define DEFAULT_MODE_NATURAL 0
#define KEY_AUTO_TIME_MODE 2
#define DEFAULT_AUTO_TIME_MODE 1
#define KEY_REVERT_COLOR_MODE 4
#define DEFAULT_REVERT_COLOR_MODE 1

/* messages handling */
 void load_persist();
 void save_persist();

 void inbox_received_callback(DictionaryIterator *iterator, void *context);
 void inbox_dropped_callback(AppMessageResult reason, void *context);
 void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
 void outbox_sent_callback(DictionaryIterator *iterator, void *context);


static bool date_mode = 0;
static bool rounded_mode;
static bool natural_mode;
static bool auto_time_mode;
static bool revert_color_mode;


#endif /* SRC_SETTINGS_H_ */
