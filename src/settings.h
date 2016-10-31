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
#define DEFAULT_MODE_ROUNDED FALSE
#define KEY_MODE_NATURAL 3
#define DEFAULT_MODE_NATURAL TRUE
#define KEY_AUTO_TIME_MODE 2
#define DEFAULT_AUTO_TIME_MODE TRUE
#define KEY_REVERT_COLOR_MODE 4
#define DEFAULT_REVERT_COLOR_MODE TRUE
#define KEY_24H_MODE 5
#define DEFAULT_24H_MODE FALSE

/* persistent storage */
 void load_persist();
 void save_persist();

 /* messages handling */
 void inbox_received_callback(DictionaryIterator *iterator, void *context);
 void inbox_dropped_callback(AppMessageResult reason, void *context);
 void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
 void outbox_sent_callback(DictionaryIterator *iterator, void *context);

/* settigngs */
extern bool rounded_mode;		// if true round minutes do the closest 5
extern bool natural_mode;		// if true does 0-30 minutes
extern bool auto_time_mode;		// if true return to time automatically after seeing the date
extern bool revert_color_mode;	// if true background is black, white otherwise
extern bool hours_24h_mode;		// if true uses 24h mode

#endif /* SRC_SETTINGS_H_ */
