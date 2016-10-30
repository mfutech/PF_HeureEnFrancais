/*
 * main.h
 *
 *  Created on: 29 oct. 2016
 *      Author: Marc
 */

#pragma once

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#define TRUE 1
#define FALSE 0

void str_lower(char *str);
 void verticalAlignTextLayer(TextLayer *layer);
void mark_text_as_dirty();
void bld_text_layer_set_text(TextLayer *t_layer, char *str);
void reg_text_layer_set_text(TextLayer *t_layer, char *str);
void set_text_color(bool revert);
 void show_date();
void show_hours(int hour_ref);
void show_minutes_30(int min_ref, bool pile, bool need_minus);
void show_minutes_60(int min_ref, bool pile);
 void show_time();
 void update_time();

/* Callbacks */
 void tick_handler(struct tm *tick_time, TimeUnits units_changed);
 void tap_handler(AccelAxisType axis, int32_t direction);


 void main_window_load(Window *window);
 void main_window_unload(Window *window);



#endif /* SRC_MAIN_H_ */
