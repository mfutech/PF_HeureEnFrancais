/***
 *** main.c
 ***
 ***
 *** 2015 MFU Tech 
 *** source code: https://github.com/mfutech/PF_HeureEnFrancais
 *** website:     http://mfutech.github.io/PF_HeureEnFrancais/
 ***/

#include <pebble.h>
#include <ctype.h>
#include "logging.h"
#include "french_number.h"
#include "main.h"
#include "settings.h"


/*_ --------------- Macros ----------------------- */

static Window *s_main_window;
static TextLayer *s_line1_layer;
static TextLayer *s_line2_layer;
static TextLayer *s_line3_layer;
static TextLayer *s_line4_layer;

static GFont s_big_font;
static GFont s_medium_font;
static GFont s_small_font;
static GFont s_tiny_font;
static GFont s_bld_big_font;
static GFont s_bld_medium_font;
static GFont s_bld_small_font;
static GFont s_bld_tiny_font;

#define LINE_LEN 15
static char s_line1[LINE_LEN];
static char s_line2[LINE_LEN];
static char s_line3[LINE_LEN];
static char s_line4[LINE_LEN];

static bool date_mode = 0;			// internal state, if true showing the date

/*_ --------------- utility functions -------------*/

void str_lower(char *str){
  int i = 0;
  while(str[i]){
    str[i] = tolower((int)str[i]);
    i++;
  }
}

int round_to_5(int min) {
  return ((min+2)/5)*5;
}

/*_ --------------- text management -------------- */

 void verticalAlignTextLayer(TextLayer *layer) {
  GRect frame = layer_get_frame(text_layer_get_layer(layer));
  GSize content = text_layer_get_content_size(layer);
  layer_set_frame(text_layer_get_layer(layer),
		  GRect(frame.origin.x,
			frame.origin.y + (frame.size.h - content.h - 5) / 2, 
			frame.size.w, content.h));
}

void mark_text_as_dirty() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter mark_text_as_dirty");
  layer_set_hidden((Layer*)text_layer_get_layer(s_line1_layer), true);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line2_layer), true);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line3_layer), true);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line4_layer), true);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line1_layer), false);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line2_layer), false);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line3_layer), false);
  layer_set_hidden((Layer*)text_layer_get_layer(s_line4_layer), false);
  layer_mark_dirty((Layer*)text_layer_get_layer(s_line1_layer));
  layer_mark_dirty((Layer*)text_layer_get_layer(s_line2_layer));
  layer_mark_dirty((Layer*)text_layer_get_layer(s_line3_layer));
  layer_mark_dirty((Layer*)text_layer_get_layer(s_line4_layer));
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit mark_text_as_dirty");
}

void bld_text_layer_set_text(TextLayer *t_layer, char *str) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter bld_text_layer_set_text");
  GSize sz;
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold text: %s", str);

  // try big font
  text_layer_set_font(t_layer, s_bld_big_font);
  text_layer_set_text(t_layer, str);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold big, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;
  
  //try medium font
  text_layer_set_font(t_layer, s_bld_medium_font);

  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold medium, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;

  //try small font
  text_layer_set_font(t_layer, s_bld_small_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold small, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;

  //try tiny font
  text_layer_set_font(t_layer, s_bld_tiny_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold tiny, sz: %d x %d", sz.h, sz.w);

  //adjust verticaly
  //verticalAlignTextLayer(t_layer);
  
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit bld_text_layer_set_text");
}


void reg_text_layer_set_text(TextLayer *t_layer, char *str) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter reg_text_layer_set_text");
  GSize sz;
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "text: %s", str);

  //try medium font
  text_layer_set_font(t_layer, s_medium_font);
  text_layer_set_text(t_layer, str);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "medium, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;

  // try small font
  text_layer_set_font(t_layer, s_small_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "small, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;

  // try tiny font
  text_layer_set_font(t_layer, s_tiny_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "tiny, sz: %d x %d", sz.h, sz.w);

  // TODO fonction a revoir, donne de mauvais résultat (carré noir)
  //adjust verticaly
  //verticalAlignTextLayer(t_layer);

  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit reg_text_layer_set_text");
}



#ifdef DEBUG
void __OLD__my_text_layer_set_text(TextLayer *t_layer, char *str) {
  //text_layer_get_content_size
  if (strlen(str) > 8)
    text_layer_set_font(t_layer, s_small_font);
  else if (strlen(str) > 9)
    text_layer_set_font(t_layer, s_tiny_font);
  else
    text_layer_set_font(t_layer, s_medium_font);
  text_layer_set_text(t_layer, str);
}
#endif

void set_text_color(bool revert) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter set_text_color");
  GColor bgcol;
  GColor fgcol1;
  GColor fgcol2;
  GColor fgcol3;
  GColor fgcol4;
  if (revert) {
    bgcol  = COLOR_FALLBACK(GColorBlack, GColorBlack);
    fgcol1 = COLOR_FALLBACK(GColorOrange, GColorWhite);
    fgcol2 = COLOR_FALLBACK(GColorYellow, GColorWhite);
    fgcol3 = COLOR_FALLBACK(GColorYellow, GColorWhite);
    fgcol4 = COLOR_FALLBACK(GColorYellow, GColorWhite);
  } else {
    bgcol = COLOR_FALLBACK(GColorWhite, GColorWhite);
    fgcol1 = COLOR_FALLBACK(GColorRed, GColorBlack);
    fgcol2 = COLOR_FALLBACK(GColorBlue, GColorBlack);
    fgcol3 = COLOR_FALLBACK(GColorBlue, GColorBlack);
    fgcol4 = COLOR_FALLBACK(GColorBlue, GColorBlack);
  }
  window_set_background_color(s_main_window, bgcol);
  text_layer_set_background_color(s_line1_layer, bgcol);
  text_layer_set_text_color(s_line1_layer, fgcol1);
  text_layer_set_background_color(s_line2_layer, bgcol);
  text_layer_set_text_color(s_line2_layer, fgcol2);
  text_layer_set_background_color(s_line3_layer, bgcol);
  text_layer_set_text_color(s_line3_layer, fgcol3);
  text_layer_set_background_color(s_line4_layer, bgcol);
  text_layer_set_text_color(s_line4_layer, fgcol4);

  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit set_text_color");
}

/*_ --------------- Date & Time function --------- */

 void show_date() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_date");
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "set local : %s", setlocale(LC_ALL, "fr_FR"));
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  /* line 1 : week day */
  strftime(s_line1, LINE_LEN, "%a", tick_time);
  str_lower(s_line1);
  bld_text_layer_set_text(s_line1_layer, french_wday[tick_time->tm_wday]);
  /* line 2 : date */
  strftime(s_line2, LINE_LEN, "%d", tick_time);
  str_lower(s_line2);
  reg_text_layer_set_text(s_line2_layer, s_line2);
  /* line 3 : month */
  strftime(s_line3, LINE_LEN, "%B", tick_time);
  str_lower(s_line3);
  reg_text_layer_set_text(s_line3_layer, french_month[tick_time->tm_mon]);
  /* line 4  : HH:MM */
  strftime(s_line4, LINE_LEN, "%H:%M", tick_time);
  str_lower(s_line4);
  reg_text_layer_set_text(s_line4_layer, s_line4);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_date");
}

/*_ --------------- Debug stuff -------------------*/

#ifdef DEBUG
 void display_debug() {
  // Get a tm structure
  bld_text_layer_set_text(s_line1_layer, "quatre");
  reg_text_layer_set_text(s_line2_layer, "dix-huit");
  reg_text_layer_set_text(s_line3_layer, "vingt-neuf");
  reg_text_layer_set_text(s_line4_layer, "vingtxneuf");
}
#endif

#if 0
 void get_config() {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}
#endif

/*_ --------------- Date & Time  ------------------*/


void show_hours(int hour_ref) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_hours");
  if (hour_ref == 0 || hour_ref == 24) {
    bld_text_layer_set_text(s_line1_layer, "minuit");
    reg_text_layer_set_text(s_line2_layer, " ");
  }
  else if (hour_ref == 12) {
    bld_text_layer_set_text(s_line1_layer, "midi");
    reg_text_layer_set_text(s_line2_layer, " ");
  }
  else {
    if (!hours_24h_mode) hour_ref = hour_ref % 12; // if not 24h mode, then modulo 12
    bld_text_layer_set_text(s_line1_layer, french_number[hour_ref + FRENCH_DIGIT_0]);
    if (hour_ref == 1)
      reg_text_layer_set_text(s_line2_layer, "heure");
    else
      reg_text_layer_set_text(s_line2_layer, "heures");
  }
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_hours");
}
void show_minutes_30(int min_ref, bool pile, bool need_minus) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_minutes_30");
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_minutes_30 - pile:%d min:%d", pile, min_ref);
  switch(min_ref) {
    case 0:
      if (pile) reg_text_layer_set_text(s_line3_layer, "pile");
      else reg_text_layer_set_text(s_line3_layer, " ");
      reg_text_layer_set_text(s_line4_layer, " ");
      break;
    case 15:
      if (need_minus) {
        reg_text_layer_set_text(s_line3_layer, "moins");
        reg_text_layer_set_text(s_line4_layer, "quart");
      }
      else {
        reg_text_layer_set_text(s_line3_layer, "et");
        reg_text_layer_set_text(s_line4_layer, "quart");
      }
      break;
    case 30:
      reg_text_layer_set_text(s_line3_layer, "et");
      reg_text_layer_set_text(s_line4_layer, "demie");
      break;
    default: 
      if (need_minus) {
        reg_text_layer_set_text(s_line3_layer, "moins");
        reg_text_layer_set_text(s_line4_layer, french_number[min_ref + FRENCH_DIGIT_0]);
      }
      else { // (tick_time->tm_min > 30) 
        reg_text_layer_set_text(s_line3_layer, french_number[min_ref + FRENCH_DIGIT_0]);
        reg_text_layer_set_text(s_line4_layer, " ");
      }  
  }
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_minutes_30");
}

void show_minutes_60(int min_ref, bool pile){
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_minutes_60");
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_minutes_60 - pile:%d min:%d", pile, min_ref);
  if (min_ref == 0) {
	  if (pile)
		  reg_text_layer_set_text(s_line3_layer, "pile");
	  else
		  reg_text_layer_set_text(s_line3_layer, " ");
	  reg_text_layer_set_text(s_line4_layer, " ");
  }
  else {
	  reg_text_layer_set_text(s_line3_layer, french_number_60[min_ref][0]);
	  reg_text_layer_set_text(s_line4_layer, french_number_60[min_ref][1]);
  }
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_minutes_60");
}

 void show_time() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_time");
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  int hour_ref = tick_time->tm_hour;
  int min_ref  = tick_time->tm_min;
  bool need_minus = false;
  bool pile = (min_ref == 0);
  
  if(rounded_mode){
    min_ref = round_to_5(min_ref);
  }  

  if (natural_mode) { 
    if (min_ref > 30) {
      min_ref = 60 - min_ref;
      hour_ref += 1;
      need_minus = true;
    }
    show_hours(hour_ref);
    show_minutes_30(min_ref, pile, need_minus);
  }
  else {
    show_hours(hour_ref);
    show_minutes_60(min_ref, pile);
  }
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_time");
}

 void update_time() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter update_time");
  if (date_mode) show_date();
  else show_time();
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit update_time");
}


/*_ --------------- Callbacks -------------------- */

/*_  -------------- Callbacks Time --------------- */

 void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (auto_time_mode) date_mode = 0;
  update_time();
}

/*_  -------------- Callbacks Accellerometer ----- */

 void tap_handler(AccelAxisType axis, int32_t direction) {
  date_mode = ! date_mode;
  update_time();
}


/*_ --------------- UI SETUP --------------------- */

 void main_window_load(Window *window) {
  // Load fonts
  s_bld_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_45));
  s_bld_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_40));
  s_bld_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_30));
  s_bld_tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_25));
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_45));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_30));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_24));
  s_tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_20));

  // Create lines TextLayer
  int y;
#if defined(PBL_RECT)
  y = 0;
  s_line1_layer = text_layer_create(GRect(1, y, 143, 53)); y += 53; /*53*/
  s_line2_layer = text_layer_create(GRect(0, y, 143, 37)); y += 37; /*90*/
  s_line3_layer = text_layer_create(GRect(2, y, 143, 37)); y += 37; /*127*/
  s_line4_layer = text_layer_create(GRect(0, y, 143, 37)); /*164*/
#elif defined(PBL_ROUND)
  y = 6;
  s_line1_layer = text_layer_create(GRect(0, y, 180, 53)); y += 53; /*53*/
  s_line2_layer = text_layer_create(GRect(0, y, 180, 37)); y += 37; /*90*/
  s_line3_layer = text_layer_create(GRect(0, y, 180, 37)); y += 37; /*127*/
  s_line4_layer = text_layer_create(GRect(0, y, 180, 37)); /*164*/
#endif

  // set alignment
#if defined(PBL_RECT) 
  text_layer_set_text_alignment(s_line1_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_line2_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(s_line3_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_line4_layer, GTextAlignmentRight);
#elif defined(PBL_ROUND)
  text_layer_set_text_alignment(s_line1_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_line2_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_line3_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_line4_layer, GTextAlignmentCenter);
#endif
  
  // set background and text colors
  text_layer_set_background_color(s_line1_layer, GColorClear);
  text_layer_set_text_color(s_line1_layer, GColorBlack);
  text_layer_set_background_color(s_line2_layer, GColorClear);
  text_layer_set_text_color(s_line2_layer, GColorBlack);
  text_layer_set_background_color(s_line3_layer, GColorClear);
  text_layer_set_text_color(s_line3_layer, GColorBlack);
  text_layer_set_background_color(s_line4_layer, GColorClear);
  text_layer_set_text_color(s_line4_layer, GColorBlack);

  // put some default text
  text_layer_set_text(s_line1_layer, "01:00");
  text_layer_set_text(s_line2_layer, "02:00");
  text_layer_set_text(s_line3_layer, "03:00");
  text_layer_set_text(s_line4_layer, "04:00");

  //Apply to TextLayer
  text_layer_set_font(s_line1_layer, s_big_font);
  text_layer_set_font(s_line2_layer, s_medium_font);
  text_layer_set_font(s_line3_layer, s_medium_font);
  text_layer_set_font(s_line4_layer, s_medium_font);
                                  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line4_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line3_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line1_layer));
}


 void main_window_unload(Window *window) {
  // Unload fonts
  fonts_unload_custom_font(s_bld_big_font);
  fonts_unload_custom_font(s_bld_medium_font);
  fonts_unload_custom_font(s_bld_small_font);
  fonts_unload_custom_font(s_bld_tiny_font);
  fonts_unload_custom_font(s_big_font);
  fonts_unload_custom_font(s_medium_font);
  fonts_unload_custom_font(s_small_font);
  fonts_unload_custom_font(s_tiny_font);

  // Destroy TextLayer
  text_layer_destroy(s_line1_layer);
  text_layer_destroy(s_line2_layer);
  text_layer_destroy(s_line3_layer);
  text_layer_destroy(s_line4_layer);
}

/*_ --------------- MAIN --------------------------*/

 void init() {
  setlocale(LC_ALL, "fr_FR");
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Register for tap events
  accel_tap_service_subscribe(tap_handler);

  // Register callbacks for App Message
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());  
  
  // get config from persistent storage
  load_persist();

  // Make sure the time is displayed from the start
  set_text_color(revert_color_mode);
  update_time();
}

 void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  save_persist();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

/*_ --------------- The END -----------------------*/

/* 
 * Local Variables:
 * mode: c
 * eval: (allout-minor-mode)
 * End:
*/

