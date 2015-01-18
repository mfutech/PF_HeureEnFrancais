#include <pebble.h>
#include <french_number.h>
  
static Window *s_main_window;
static TextLayer *s_line1_layer;
static TextLayer *s_line2_layer;
static TextLayer *s_line3_layer;

static GFont s_line1_font;
static GFont s_line2_font;
static GFont s_line3_font;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  int  hour_idx;
  
  hour_idx = tick_time->tm_hour % 12 + FRENCH_DIGIT_0;

  if (tick_time->tm_hour == 0)
    text_layer_set_text(s_line1_layer, "minuit");
  else if (tick_time->tm_hour == 12)
    text_layer_set_text(s_line1_layer, "midi");
  else
    text_layer_set_text(s_line1_layer, french_number[hour_idx]);

  if (tick_time->tm_hour == 1)
    text_layer_set_text(s_line2_layer, "heure");
  else
    text_layer_set_text(s_line2_layer, "heures");

  if (tick_time->tm_min == 0)
    text_layer_set_text(s_line3_layer, "pile");
  else if (tick_time->tm_min == 15)
    text_layer_set_text(s_line3_layer, "et quart");
  else if (tick_time->tm_min == 30)
    text_layer_set_text(s_line3_layer, "et demie");
  else if (tick_time->tm_min == 45)
    text_layer_set_text(s_line3_layer, "moins quart");
  else if (tick_time->tm_min < 30)
    text_layer_set_text(s_line3_layer, french_number[tick_time->tm_min]);
  else if (tick_time->tm_min > 30)
   text_layer_set_text(s_line3_layer, french_number[60 - tick_time->tm_min]);

}

// ------------------------------------------ Callbacks ----------------------------------------
// ------------------------------------------- UI SETUP ----------------------------------------------
static void main_window_load(Window *window) {
  // Load fonts
  s_line1_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOT_COND_BOLD_32));
  s_line2_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_24));
  s_line3_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_24));

  // Create lines TextLayer
  s_line1_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(s_line1_layer, GColorClear);
  text_layer_set_text_color(s_line1_layer, GColorBlack);
  text_layer_set_text(s_line1_layer, "01:00");

  s_line2_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_line2_layer, GColorClear);
  text_layer_set_text_color(s_line2_layer, GColorBlack);
  text_layer_set_text(s_line2_layer, "02:00");

  s_line3_layer = text_layer_create(GRect(0, 110, 144, 50));
  text_layer_set_background_color(s_line3_layer, GColorClear);
  text_layer_set_text_color(s_line3_layer, GColorBlack);
  text_layer_set_text(s_line3_layer, "03:00");
    
  //Apply to TextLayer
  text_layer_set_font(s_line1_layer, s_line1_font);
  text_layer_set_font(s_line2_layer, s_line2_font);
  text_layer_set_font(s_line3_layer, s_line3_font);
  text_layer_set_text_alignment(s_line1_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_line2_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(s_line3_layer, GTextAlignmentLeft);
                                
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_line3_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Unload fonts
  fonts_unload_custom_font(s_line1_font);
  fonts_unload_custom_font(s_line2_font);
  fonts_unload_custom_font(s_line3_font);

  // Destroy TextLayer
  text_layer_destroy(s_line1_layer);
  text_layer_destroy(s_line2_layer);
  text_layer_destroy(s_line3_layer);
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
