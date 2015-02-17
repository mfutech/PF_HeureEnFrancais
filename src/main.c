#include <pebble.h>
#include <ctype.h>
#include <french_number.h>
  
#define xxDEBUG

#ifdef DEBUG
#define MY_APP_LOG(level, fmt, args...)                                \
  app_log(level, __FILE_NAME__, __LINE__, fmt, ## args)
#else
#define MY_APP_LOG(level, fmt, args...) 
#endif

#define TRUE 1
#define FALSE 0
#define KEY_MODE_ROUNDED 1
#define DEFAULT_MODE_ROUNDED 0
#define KEY_MODE_NATURAL 3
#define DEFAULT_MODE_NATURAL 0
#define KEY_AUTO_TIME_MODE 2
#define DEFAULT_AUTO_TIME_MODE 1
#define KEY_REVERT_COLOR_MODE 4
#define DEFAULT_REVERT_COLOR_MODE 1


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

#define LINE_LEN 15
static char s_line1[LINE_LEN];
static char s_line2[LINE_LEN];
static char s_line3[LINE_LEN];
static char s_line4[LINE_LEN];

static bool date_mode = 0;
static bool rounded_mode;
static bool natural_mode;
static bool auto_time_mode;
static bool revert_color_mode;

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
  text_layer_set_font(t_layer, s_bld_big_font);
  text_layer_set_text(t_layer, str);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold big, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;
  text_layer_set_font(t_layer, s_bld_medium_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "bold medium, sz: %d x %d", sz.h, sz.w);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit bld_text_layer_set_text");
}


void reg_text_layer_set_text(TextLayer *t_layer, char *str) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter reg_text_layer_set_text");
  GSize sz;
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "text: %s", str);
  text_layer_set_font(t_layer, s_medium_font);
  text_layer_set_text(t_layer, str);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "medium, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;
  text_layer_set_font(t_layer, s_small_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "small, sz: %d x %d", sz.h, sz.w);
  if (sz.w < 120) return;
  text_layer_set_font(t_layer, s_tiny_font);
  sz = text_layer_get_content_size(t_layer);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "tiny, sz: %d x %d", sz.h, sz.w);
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
	GColor bgcol = revert ? GColorBlack : GColorWhite;
	GColor fgcol = revert ? GColorWhite : GColorBlack;
	window_set_background_color(s_main_window, bgcol);
	text_layer_set_background_color(s_line1_layer, bgcol);
	text_layer_set_text_color(s_line1_layer, fgcol);
	text_layer_set_background_color(s_line2_layer, bgcol);
	text_layer_set_text_color(s_line2_layer, fgcol);
	text_layer_set_background_color(s_line3_layer, bgcol);
	text_layer_set_text_color(s_line3_layer, fgcol);
	text_layer_set_background_color(s_line4_layer, bgcol);
	text_layer_set_text_color(s_line4_layer, fgcol);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit set_text_color");
}

static void show_date() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter show_date");
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "set local : %s", setlocale(LC_ALL, "fr_FR"));
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  strftime(s_line1, LINE_LEN, "%a", tick_time);
  str_lower(s_line1);
  bld_text_layer_set_text(s_line1_layer, s_line1);
  strftime(s_line2, LINE_LEN, "%d", tick_time);
  str_lower(s_line2);
  reg_text_layer_set_text(s_line2_layer, s_line2);
  strftime(s_line3, LINE_LEN, "%B", tick_time);
  str_lower(s_line3);
  reg_text_layer_set_text(s_line3_layer, s_line3);
  strftime(s_line4, LINE_LEN, "%H:%M", tick_time);
  str_lower(s_line4);
  reg_text_layer_set_text(s_line4_layer, s_line4);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_date");
}
#ifdef DEBUG
static void display_debug() {
  // Get a tm structure
  bld_text_layer_set_text(s_line1_layer, "quatre");
  reg_text_layer_set_text(s_line2_layer, "dix-huit");
  reg_text_layer_set_text(s_line3_layer, "vingt-neuf");
  reg_text_layer_set_text(s_line4_layer, "vingtxneuf");
}
#endif
#if 0
static void get_config() {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}
#endif

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
    hour_ref = hour_ref % 12;
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
  switch(min_ref) {
    case 0:
      if (pile) reg_text_layer_set_text(s_line3_layer, "pile");
      else reg_text_layer_set_text(s_line3_layer, "");
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
	if (pile) {
		reg_text_layer_set_text(s_line3_layer, "pile");
		reg_text_layer_set_text(s_line4_layer, " ");
		return;
	}
  else {
	  reg_text_layer_set_text(s_line3_layer, french_number_60[min_ref][0]);
	  reg_text_layer_set_text(s_line4_layer, french_number_60[min_ref][1]);
  }
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit show_minutes_60");
}

static void show_time() {
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
static void update_time() {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "enter update_time");
  if (date_mode) show_date();
  else show_time();
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "exit update_time");
}

// ------------------------------------------ Manage persistent storage
static void load_persist() {
  rounded_mode = persist_exists(KEY_MODE_ROUNDED) ? persist_read_bool(KEY_MODE_ROUNDED) : DEFAULT_MODE_ROUNDED;
  natural_mode = persist_exists(KEY_MODE_NATURAL) ? persist_read_bool(KEY_MODE_NATURAL) : DEFAULT_MODE_NATURAL;
  auto_time_mode = persist_exists(KEY_AUTO_TIME_MODE) ? persist_read_bool(KEY_AUTO_TIME_MODE) : DEFAULT_AUTO_TIME_MODE;
  revert_color_mode = persist_exists(KEY_REVERT_COLOR_MODE) ? persist_read_bool(KEY_REVERT_COLOR_MODE) : DEFAULT_REVERT_COLOR_MODE;
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "natural_mode: %d\n", natural_mode);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}

static void save_persist() {
  persist_write_bool(KEY_MODE_NATURAL, natural_mode);
  persist_write_bool(KEY_AUTO_TIME_MODE, auto_time_mode);
  persist_write_bool(KEY_MODE_ROUNDED, rounded_mode);
  persist_write_bool(KEY_REVERT_COLOR_MODE, revert_color_mode);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "natural_mode: %d\n", natural_mode);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}

// ------------------------------------------ Callbacks ----------------------------------------

// ------------------------------------------ Callbacks Time ----------------------------------------

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (auto_time_mode) date_mode = 0;
  update_time();
}

// ------------------------------------------ Callbacks Accellerometer ----------------------------------------

static void tap_handler(AccelAxisType axis, int32_t direction) {
  date_mode = ! date_mode;
  update_time();
}

// ------------------------------------------ Callbacks Messages ----------------------------------------


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received");
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_MODE_NATURAL:
		natural_mode = ((bool)atoi(t->value->cstring));
		MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "natural_mode: %d\n", (int)atoi(t->value->cstring));
    break;
    case KEY_AUTO_TIME_MODE:
		auto_time_mode = (bool)atoi(t->value->cstring);
		MY_APP_LOG(APP_LOG_LEVEL_DEBUG, "auto_time_mode: %d\n", (int)atoi(t->value->cstring));
	break;
    case KEY_MODE_ROUNDED:
		rounded_mode = ((bool)atoi(t->value->cstring));
		MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "rounded_mode: %d\n", (int)atoi(t->value->cstring));
	break;
    case KEY_REVERT_COLOR_MODE:
		revert_color_mode = ((bool)atoi(t->value->cstring));
		MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "revert_color_mode: %d\n", (int)atoi(t->value->cstring));
	break;
    default:
      MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  set_text_color(revert_color_mode);
  update_time();
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "natural_mode: %d\n", natural_mode);
  MY_APP_LOG(APP_LOG_LEVEL_DEBUG,  "auto_time_mode: %d\n", auto_time_mode);
}


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  MY_APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  MY_APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

// ------------------------------------------- UI SETUP ----------------------------------------------
static void main_window_load(Window *window) {
  // Load fonts
  s_bld_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_45));
  s_bld_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_40));
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_BOLD_45));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_30));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_24));
  s_tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONFORTAA_20));
  // Create lines TextLayer
  int y = 0;
  s_line1_layer = text_layer_create(GRect(1, y, 143, 53)); y += 53; /*53*/
  text_layer_set_text_alignment(s_line1_layer, GTextAlignmentLeft);
  text_layer_set_background_color(s_line1_layer, GColorClear);
  text_layer_set_text_color(s_line1_layer, GColorBlack);
  text_layer_set_text(s_line1_layer, "01:00");

  s_line2_layer = text_layer_create(GRect(0, y, 143, 37)); y += 37; /*90*/
  text_layer_set_text_alignment(s_line2_layer, GTextAlignmentRight);
  text_layer_set_background_color(s_line2_layer, GColorClear);
  text_layer_set_text_color(s_line2_layer, GColorBlack);
  text_layer_set_text(s_line2_layer, "02:00");

  s_line3_layer = text_layer_create(GRect(2, y, 143, 37)); y += 37; /*127*/
  text_layer_set_text_alignment(s_line3_layer, GTextAlignmentLeft);
  text_layer_set_background_color(s_line3_layer, GColorClear);
  text_layer_set_text_color(s_line3_layer, GColorBlack);
  text_layer_set_text(s_line3_layer, "03:00");

  s_line4_layer = text_layer_create(GRect(0, y, 143, 37)); /*164*/
  text_layer_set_text_alignment(s_line4_layer, GTextAlignmentRight);
  text_layer_set_background_color(s_line4_layer, GColorClear);
  text_layer_set_text_color(s_line4_layer, GColorBlack);
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


static void main_window_unload(Window *window) {
  // Unload fonts
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

static void init() {
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

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  save_persist();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
