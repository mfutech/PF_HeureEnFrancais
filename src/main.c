#include <pebble.h>
#include <french_number.h>
  
#define COOLVETICAFONT 1
#define KENYANCOFFEEFONT 0
  
static Window *s_main_window;
static TextLayer *s_line1_layer;
static TextLayer *s_line2_layer;
static TextLayer *s_line3_layer;
static TextLayer *s_line4_layer;

static GFont s_big_font;
static GFont s_medium_font;
static GFont s_small_font;
static GFont s_tiny_font;

#define LINE_LEN 15
static char s_line1[LINE_LEN];
static char s_line2[LINE_LEN];
static char s_line3[LINE_LEN];
static char s_line4[LINE_LEN];

static bool date_mode = 0;

void my_text_layer_set_text(TextLayer *t_layer, char *str) {
  if (strlen(str) > 8)
    text_layer_set_font(t_layer, s_small_font);
  else if (strlen(str) > 9)
    text_layer_set_font(t_layer, s_tiny_font);
  else
    text_layer_set_font(t_layer, s_medium_font);
  text_layer_set_text(t_layer, str);
}

static void show_date() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  text_layer_set_text(s_line1_layer, "date");
  strftime(s_line2, LINE_LEN, "%d.%m.%Y", tick_time);
  my_text_layer_set_text(s_line2_layer, s_line2);
  strftime(s_line3, LINE_LEN, "heure", tick_time);
  my_text_layer_set_text(s_line3_layer, s_line3);
  strftime(s_line4, LINE_LEN, "%H:%M", tick_time);
  my_text_layer_set_text(s_line4_layer, s_line4);

}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  int hour_ref = tick_time->tm_hour;
  int min_ref  = tick_time->tm_min;
  bool need_minus = false;
  
  if (min_ref > 30) {
    min_ref = 60 - min_ref;
    hour_ref += 1;
    need_minus = true;
  }
  
  if (hour_ref == 0 || hour_ref == 24) {
    text_layer_set_text(s_line1_layer, "minuit");
    text_layer_set_text(s_line2_layer, " ");
  }
  else if (hour_ref == 12) {
    text_layer_set_text(s_line1_layer, "midi");
    text_layer_set_text(s_line2_layer, " ");
  }
  else {
    hour_ref = hour_ref % 12;
    text_layer_set_text(s_line1_layer, french_number[hour_ref + FRENCH_DIGIT_0]);
    if (hour_ref == 1)
      text_layer_set_text(s_line2_layer, "heure");
    else
      text_layer_set_text(s_line2_layer, "heures");
  }
  switch(min_ref) {
    case 0:
      my_text_layer_set_text(s_line3_layer, "pile");
      my_text_layer_set_text(s_line4_layer, " ");
      break;
    case 15:
      if (need_minus) {
        my_text_layer_set_text(s_line3_layer, "moins");
        my_text_layer_set_text(s_line4_layer, "quart");
      }
      else {
        my_text_layer_set_text(s_line3_layer, "et");
        my_text_layer_set_text(s_line4_layer, "quart");
      }
      break;
    case 30:
      text_layer_set_text(s_line3_layer, "et");
      text_layer_set_text(s_line4_layer, "demie");
      break;
    default: 
      if (need_minus) {
        my_text_layer_set_text(s_line3_layer, "moins");
        my_text_layer_set_text(s_line4_layer, french_number[min_ref + FRENCH_DIGIT_0]);
      }
      else { // (tick_time->tm_min > 30) 
        my_text_layer_set_text(s_line3_layer, french_number[min_ref + FRENCH_DIGIT_0]);
        my_text_layer_set_text(s_line4_layer, " ");
      }  
  }
}

// ------------------------------------------ Callbacks ----------------------------------------

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void tap_handler(AccelAxisType axis, int32_t direction) {
  date_mode = ! date_mode;
  if (date_mode) show_date();
  else update_time();
}
// ------------------------------------------- UI SETUP ----------------------------------------------
static void main_window_load(Window *window) {
  // Load fonts
#if COOLVETICAFONT
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COOLVETICA_48));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COOLVETICA_34));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COOLVETICA_28));
  s_tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COOLVETICA_22));
#endif
#if KENYANCOFFEEFONT
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KENYAN_COFFEE_48));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KENYAN_COFFEE_34));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KENYAN_COFFEE_28));
  s_tiny_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_KENYAN_COFFEE_24));
#endif
  // Create lines TextLayer
  int y = 2;
  s_line1_layer = text_layer_create(GRect(2, y, 140, 53));
  y += 53;
  text_layer_set_background_color(s_line1_layer, GColorClear);
  text_layer_set_text_color(s_line1_layer, GColorBlack);
  text_layer_set_text(s_line1_layer, "01:00");

  s_line2_layer = text_layer_create(GRect(2, y, 140, 37));
  y += 37;
  text_layer_set_background_color(s_line2_layer, GColorClear);
  text_layer_set_text_color(s_line2_layer, GColorBlack);
  text_layer_set_text(s_line2_layer, "02:00");

  s_line3_layer = text_layer_create(GRect(2, y, 140, 37));
  y += 37;
  text_layer_set_background_color(s_line3_layer, GColorClear);
  text_layer_set_text_color(s_line3_layer, GColorBlack);
  text_layer_set_text(s_line3_layer, "03:00");

  s_line4_layer = text_layer_create(GRect(2, y, 140, 37));
  text_layer_set_background_color(s_line4_layer, GColorClear);
  text_layer_set_text_color(s_line4_layer, GColorBlack);
  text_layer_set_text(s_line4_layer, "04:00");

  //Apply to TextLayer
  text_layer_set_font(s_line1_layer, s_big_font);
  text_layer_set_font(s_line2_layer, s_medium_font);
  text_layer_set_font(s_line3_layer, s_medium_font);
  text_layer_set_font(s_line4_layer, s_medium_font);
  
  text_layer_set_text_alignment(s_line1_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_line2_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(s_line3_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_line4_layer, GTextAlignmentRight);
                                
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line3_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line4_layer));
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
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Register for tap events
  accel_tap_service_subscribe(tap_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
