#include <pebble.h>
#include <stdio.h>

Window *window;
TextLayer *hora_layer;
TextLayer *bateria_layer;
TextLayer *fecha_layer;
TextLayer *dia_layer;
TextLayer *mes_layer;
TextLayer *icoCarga_layer;
TextLayer *icoBlut_layer;
Layer *linea_layer;
Layer *cuadro_layer;

static const char *dias[] = {
  "  Domingo", "  Lunes", "  Martes", "  Miércoles", "  Jueves", "  Viernes", "  Sábado"
};

static const char *meses[] = {
  "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", 
  "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
};

// variables temporales para hora y día
char buffer_time[] = "00:00";
char buffer_day[] = "00/00/00";

//posiciones líneas, recuadros y otros
int posY = 30;
int posY_linea = 30;
int posY_bat = 6;
int posX_bat = 100;

//relleno (negro=1, blanco = 0)
int relleno=1; 

void line_layer_update_callback(Layer *layer, GContext* ctx) 
{
	GRect d = layer_get_bounds(layer);	
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
	if (relleno==0) {
  		graphics_context_set_fill_color(ctx, GColorWhite);
    	GRect rect = GRect(d.origin.x+1,d.origin.y+1,d.size.w-2,d.size.h-2);
    	graphics_fill_rect(ctx, rect, 0, GCornerNone);
	}
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    
	//Control de la carga de la batería
	static char buffer_b[] = "100%";
	BatteryChargeState state = battery_state_service_peek();
	if (state.is_charging) {
	//	GFont awesome_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AWESOME_18));
	//	icoCarga_layer = text_layer_create((GRect) { .origin = { 10, 0 }, .size = { 30, 30 } });
	//	text_layer_set_background_color(icoCarga_layer, GColorBlack); // Change background color
	//	text_layer_set_text_color(icoCarga_layer, GColorWhite); // Change foreground color
	//	text_layer_set_text_alignment(icoCarga_layer, GTextAlignmentCenter);
	//	text_layer_set_font(icoCarga_layer, awesome_font);
	//	text_layer_set_text(icoCarga_layer, "\ue600"); // Here set the unicode key
		strcpy(buffer_b, "×××");
	} else {
	  snprintf(buffer_b, sizeof("100%"), "%d%%", state.charge_percent);
	}
	text_layer_set_text(bateria_layer, buffer_b);
	
	static char txt_dia[] = "             ";
	static char txt_mes[] = "            ";  
	int dia = tick_time->tm_wday;
	int mes = tick_time->tm_mon;

	strcpy(txt_dia, dias[dia]);
	text_layer_set_text(dia_layer, txt_dia);
	
	strcpy(txt_mes, meses[mes]);	  
	//text_layer_set_text(mes_layer, txt_mes);
	
	strftime(buffer_day,sizeof("00/00/00"),"%d-%m-%y",tick_time);
	text_layer_set_text(fecha_layer,buffer_day);
  
  strftime(buffer_time,sizeof("00:00"),"%H:%M",tick_time);
	text_layer_set_text(hora_layer,buffer_time);
}

void up_click_handler(ClickRecognizerRef recognizer, void *context)
{

}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{

}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{

}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void window_load(Window *window)
{
	hora_layer = text_layer_create(GRect(0, posY, 144, 64));
	text_layer_set_background_color(hora_layer, GColorBlack);
	text_layer_set_text_color(hora_layer, GColorClear);
	text_layer_set_text_alignment(hora_layer, GTextAlignmentCenter);
	text_layer_set_font(hora_layer, fonts_get_system_font( FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	
	dia_layer = text_layer_create(GRect(0, posY+60, 144, 29));
	text_layer_set_background_color(dia_layer, GColorClear);
	text_layer_set_text_color(dia_layer, GColorBlack);
	text_layer_set_text_alignment(dia_layer, GTextAlignmentLeft);
	text_layer_set_font(dia_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	
	fecha_layer = text_layer_create(GRect(0, posY+86, 144, 52));
	text_layer_set_background_color(fecha_layer, GColorClear);
	text_layer_set_text_color(fecha_layer, GColorBlack);
	text_layer_set_text_alignment(fecha_layer, GTextAlignmentCenter);
	text_layer_set_font(fecha_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	
	layer_add_child(window_get_root_layer(window), (Layer*) hora_layer);
	
	layer_add_child(window_get_root_layer(window), (Layer*) dia_layer);
	
layer_add_child(window_get_root_layer(window), (Layer*) fecha_layer);

// linea
	 GRect line_frame = GRect(8, posY+90, 131, 2);
	 linea_layer = layer_create(line_frame);
	 layer_set_update_proc(linea_layer, line_layer_update_callback);
	 layer_add_child(window_get_root_layer(window), (Layer*) linea_layer);
	 
// sección batería
//-----------------

// cuerpo batería
  	relleno=0;
  	GRect cuadro_frame = GRect(posX_bat, posY_bat, 29, 14);
  	cuadro_layer = layer_create(cuadro_frame);
  	layer_set_update_proc(cuadro_layer, line_layer_update_callback);
  	layer_add_child(window_get_root_layer(window), (Layer*) cuadro_layer);

// polo batería
	relleno=0;
	cuadro_frame = GRect(posX_bat+28, posY_bat+5, 4, 4);
	cuadro_layer = layer_create(cuadro_frame);
	layer_set_update_proc(cuadro_layer, line_layer_update_callback);
	layer_add_child(window_get_root_layer(window), (Layer*) cuadro_layer);

//texto carga batería
	bateria_layer = text_layer_create(GRect(posX_bat+2, posY_bat-3, 38, 17));
	text_layer_set_background_color(bateria_layer, GColorClear);
	text_layer_set_text_color(bateria_layer, GColorBlack);
	text_layer_set_font(bateria_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(bateria_layer));
	
	//layer_add_child(window_get_root_layer(window), text_layer_get_layer(icoCarga_layer));
	
// -----------------

struct tm *t;
	time_t temp;	
	temp = time(NULL);	
	t = localtime(&temp);	

	//Manually call the tick handler when the window is loading
	tick_handler(t, MINUTE_UNIT);
}

void window_unload(Window *window)
{
	text_layer_destroy(hora_layer);
	text_layer_destroy(fecha_layer);
	text_layer_destroy(bateria_layer);
	text_layer_destroy(dia_layer);
	text_layer_destroy(mes_layer);
	layer_destroy(linea_layer);
	layer_destroy(cuadro_layer);
	text_layer_destroy(icoCarga_layer);
	text_layer_destroy(icoBlut_layer);
}

void init()
{
	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	window_set_click_config_provider(window, click_config_provider);
	window_stack_push(window,true);
}

void deinit()
{
	tick_timer_service_unsubscribe();
	window_destroy(window);
}


int main(void)
{
	init();
	app_event_loop();
	deinit();
}
