/*
 * settings.c
 *
 *  Created on: 29 oct. 2016
 *      Author: Marc
 */

#include <pebble.h>
#include <ctype.h>
#include "settings.h"
#include "logging.h"
#include "main.h"


/*_  -------------- Manage persistent storage ---------------*/

void load_persist() {
	rounded_mode = persist_exists(KEY_MODE_ROUNDED) ? persist_read_bool(KEY_MODE_ROUNDED) : DEFAULT_MODE_ROUNDED;
	natural_mode = persist_exists(KEY_MODE_NATURAL) ? persist_read_bool(KEY_MODE_NATURAL) : DEFAULT_MODE_NATURAL;
	auto_time_mode = persist_exists(KEY_AUTO_TIME_MODE) ? persist_read_bool(KEY_AUTO_TIME_MODE) : DEFAULT_AUTO_TIME_MODE;
	revert_color_mode = persist_exists(KEY_REVERT_COLOR_MODE) ? persist_read_bool(KEY_REVERT_COLOR_MODE) : DEFAULT_REVERT_COLOR_MODE;
	hours_24h_mode = persist_exists(KEY_24H_MODE) ? persist_read_bool(KEY_24H_MODE) : DEFAULT_24H_MODE;
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "natural_mode: %d\n", natural_mode);
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}

void save_persist() {
	persist_write_bool(KEY_MODE_NATURAL, natural_mode);
	persist_write_bool(KEY_AUTO_TIME_MODE, auto_time_mode);
	persist_write_bool(KEY_MODE_ROUNDED, rounded_mode);
	persist_write_bool(KEY_REVERT_COLOR_MODE, revert_color_mode);
	persist_write_bool(KEY_24H_MODE, hours_24h_mode);
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "natural_mode: %d\n", natural_mode);
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}

/*_  -------------- Callbacks Messages ----------- */


void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received");
	// Read first item
	Tuple *t = dict_read_first(iterator);

	// For all items
	while(t != NULL) {
		// Which key was received?
		if (t->key ==  MESSAGE_KEY_NaturalMode) {
			natural_mode = (bool)(t->value->int32 == 1);
			MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "natural_mode: %d\n", (int)natural_mode);
		} else
			if (t->key == MESSAGE_KEY_AutoReturnToTime) {
				auto_time_mode = (bool)(t->value->int32 == 1);
				MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "auto_time_mode: %d\n", (int)auto_time_mode);
			} else
				if (t->key == MESSAGE_KEY_MinuteMode) {
					rounded_mode = (bool)(atoi(t->value->cstring) == 30);
					MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "rounded_mode: %d\n", (int)atoi(t->value->cstring));
				} else
					if (t->key == MESSAGE_KEY_BackgroudColor) {
						revert_color_mode = (bool)(atoi(t->value->cstring) == 1);
						MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "revert_color_mode: %d\n", (int)atoi(t->value->cstring));
					} else
						if (t->key == MESSAGE_KEY_TwentyFourMode) {
							hours_24h_mode = (bool)(t->value->int32 == 1);
							MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "24h display mode: %d\n", (int)hours_24h_mode);
						} else
						{
							MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
						}

		// Look for next item
		t = dict_read_next(iterator);
	}
	set_text_color(revert_color_mode);
	update_time();
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "natural_mode: %d\n", natural_mode);
	MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}


void inbox_dropped_callback(AppMessageResult reason, void *context) {
	MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	MY_APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
