#include <pebble.h>

Window *window;	
TextLayer *x_axis_layer;
TextLayer *y_axis_layer;
TextLayer *z_axis_layer;
	
// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	X_AXIS_KEY = 1,
	Y_AXIS_KEY = 2,
	Z_AXIS_KEY = 3
};

static uint32_t x_axis_value;
static uint32_t y_axis_value;
static uint32_t z_axis_value;

// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, STATUS_KEY, 0x1);
	dict_write_uint32(iter, X_AXIS_KEY, x_axis_value);
	dict_write_uint32(iter, Y_AXIS_KEY, y_axis_value);
	dict_write_uint32(iter, Z_AXIS_KEY, z_axis_value);
	
	dict_write_end(iter);
  	app_message_outbox_send();
}

static void data_handler(AccelData *data, uint32_t num_samples) {
	x_axis_value = data[0].x;
	y_axis_value = data[0].y;
	z_axis_value = data[0].z;
	
	static char x_axis_text[16];
	static char y_axis_text[16];
	static char z_axis_text[16];
	
	snprintf(x_axis_text, sizeof(x_axis_text), "X: %d", (int)x_axis_value);
	snprintf(y_axis_text, sizeof(y_axis_text), "Y: %d", (int)y_axis_value);
	snprintf(z_axis_text, sizeof(z_axis_text), "Z: %d", (int)z_axis_value);
	
	// Set the text, font, and text alignment
	text_layer_set_text(x_axis_layer, x_axis_text);
	text_layer_set_text(y_axis_layer, y_axis_text);
	text_layer_set_text(z_axis_layer, z_axis_text);
	
	// Send message to REST service
	send_message();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
	window = window_create();
	
	//set up text layers for x, y and z axis data.
	x_axis_layer = text_layer_create(GRect(10, 20, 100, 25));
	y_axis_layer = text_layer_create(GRect(10, 50, 100, 25));
	z_axis_layer = text_layer_create(GRect(10, 80, 100, 25));
	
	text_layer_set_font(x_axis_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(x_axis_layer, GTextAlignmentCenter);
	text_layer_set_font(y_axis_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(y_axis_layer, GTextAlignmentCenter);
	text_layer_set_font(z_axis_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(z_axis_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(x_axis_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(y_axis_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(z_axis_layer));
	
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	int num_samples = 1;
	accel_data_service_subscribe(num_samples, data_handler);
	
	// Choose update rate
	accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

void deinit(void) {
	app_message_deregister_callbacks();
	accel_data_service_unsubscribe();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}