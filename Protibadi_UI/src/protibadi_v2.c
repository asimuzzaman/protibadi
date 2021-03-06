#include "protibadi_v2.h"

appdata_s *ad; //Global app data Struct

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
list_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *it = event_info;
	elm_list_item_selected_set(it, EINA_FALSE);
}

static void my_table_pack(Evas_Object *table, Evas_Object *child, int x, int y, int w, int h) //
{
	evas_object_size_hint_align_set(child, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(child, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_table_pack(table, child, x, y, w, h);
	evas_object_show(child);
}

static void
add_contact_ui(void *data) {

	Evas_Object *box = elm_box_add(ad->conform);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
	elm_object_content_set(ad->conform, box);
	evas_object_show(box);

	/* Table */
	Evas_Object *table = elm_table_add(ad->conform);
	/* Make table homogenous - every cell will be the same size */
	elm_table_homogeneous_set(table, EINA_TRUE);
	/* Set padding of 10 pixels multiplied by scale factor of UI */
	elm_table_padding_set(table, 10 * elm_config_scale_get(), 10 * elm_config_scale_get());
	/* Let the table child allocation area expand within in the box */
	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	/* Set table to fiill width but align to bottom of box */
	evas_object_size_hint_align_set(table, EVAS_HINT_FILL, /*EVAS_HINT_FILL*/0.05);
	elm_box_pack_end(box, table);
	evas_object_show(table);

	/* Bg-1 */
	Evas_Object *bg = elm_bg_add(ad->conform);
	elm_bg_color_set(bg, 210, 210, 210);
	my_table_pack(table, bg, 0, 0, 3, 1);

	/* Entry-1 */
	ad->entry1 = elm_entry_add(ad->conform);
	elm_object_part_text_set(ad->entry1, "elm.guide", "Name");
	my_table_pack(table, ad->entry1, 0, 0, 3, 1);

	/* Bg-2 */
	bg = elm_bg_add(ad->conform);
	elm_bg_color_set(bg, 210, 210, 210);
	my_table_pack(table, bg, 0, 1, 3, 1);

	/* Entry-2 */
	ad->entry2 = elm_entry_add(ad->conform);
	elm_object_part_text_set(ad->entry2, "elm.guide", "Number");
	my_table_pack(table, ad->entry2, 0, 1, 3, 1);

	/* Bg-3 */
	bg = elm_bg_add(ad->conform);
	elm_bg_color_set(bg, 210, 210, 210);
	my_table_pack(table, bg, 0, 2, 3, 1);

	/* Entry-3 */
	ad->entry3 = elm_entry_add(ad->conform);
	elm_object_part_text_set(ad->entry3, "elm.guide", "Email");
	my_table_pack(table, ad->entry3, 0, 2, 3, 1);

	/* Button-Add */
	Evas_Object *btn = elm_button_add(ad->conform);
	elm_object_text_set(btn, "Add");
	evas_object_smart_callback_add(btn, "clicked", btn_add_cb, ad); //send to SERVICE APP
	//my_table_pack(table, btn, 0, 1, 1, 1);
	my_table_pack(table, btn, 1, 3, 1, 1);
}

static void
create_base_gui(appdata_s *ad) {
	// Setting the window
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

    /* Create conformant */
    ad->conform = elm_conformant_add(ad->win);

    evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->conform);
    evas_object_show(ad->conform);

    // Create a naviframe
    ad->navi = elm_naviframe_add(ad->conform);
    evas_object_size_hint_align_set(ad->navi, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(ad->navi, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    elm_object_content_set(ad->conform, ad->navi);
    evas_object_show(ad->navi);

    //create list
    Evas_Object *list = elm_list_add(ad->navi);
    elm_list_mode_set(list, ELM_LIST_COMPRESS);
    evas_object_smart_callback_add(list, "selected", list_selected_cb, NULL);

    elm_list_item_append(list, "Add contact", NULL, NULL, add_contact_ui, ad->navi);
    elm_list_item_append(list, "Show contacts", NULL, NULL, NULL, ad->navi);
    elm_list_item_append(list, "Enable receiver", NULL, NULL, NULL, ad->navi);
    elm_list_item_append(list, "Ask for help", NULL, NULL, ask_for_help, ad->navi);
	elm_list_go(list);

	elm_naviframe_item_push(ad->navi, "Protibadi", NULL, NULL, list, NULL);

	evas_object_show(ad->win); //
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	ad = data;

	create_base_gui(ad);

	launch_service_app();

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
