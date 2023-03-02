#include <stdio.h>
#include <dbus/dbus.h>

int main() {
    DBusError error;
    DBusConnection *conn;
    dbus_uint32_t serial = 0;
    char *app_name = "My App";
    dbus_uint32_t replaces_id = 0;
    char *app_icon = "";
    char *summary = "Notification from My App";
    char *body = "This is a test notification from My App.";
    char **actions = NULL;
    dbus_int32_t timeout = -1;
    dbus_uint32_t notification_id = 0;

    dbus_error_init(&error);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Error connecting to the D-Bus session bus: %s\n", error.message);
        dbus_error_free(&error);
        return 1;
    }

    DBusMessage *message = dbus_message_new_method_call("org.freedesktop.Notifications",
                                                        "/org/freedesktop/Notifications",
                                                        "org.freedesktop.Notifications",
                                                        "Notify");
    DBusMessageIter args, array;
    dbus_message_iter_init_append(message, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &app_name);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &notification_id);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &app_icon);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &summary);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &body);
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &array);
    dbus_message_iter_close_container(&args, &array);
	DBusMessageIter dict_entry;
	const char *key = "";
	dbus_uint32_t value = 0;
	dbus_message_iter_open_container(&args, DBUS_TYPE_DICT_ENTRY, NULL, &dict_entry);
	dbus_message_iter_append_basic(&dict_entry, DBUS_TYPE_STRING, &key);
	DBusMessageIter variant;
	dbus_message_iter_open_container(&dict_entry, DBUS_TYPE_VARIANT, DBUS_TYPE_UINT32_AS_STRING, &variant);
	dbus_message_iter_append_basic(&variant, DBUS_TYPE_UINT32, &value);
	dbus_message_iter_close_container(&dict_entry, &variant);
	dbus_message_iter_close_container(&args, &dict_entry);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &timeout);
	DBusPendingCall* pending;
	
    dbus_connection_send_with_reply(conn, message, &pending, DBUS_TIMEOUT_INFINITE);
    dbus_connection_flush(conn);
    dbus_message_unref(message);
	dbus_pending_call_block(pending);
	message = dbus_pending_call_steal_reply(pending);
	dbus_pending_call_unref(pending);
	DBusMessageIter args2;
	int arg, id;
	if(!dbus_message_iter_init(message, &args2))
		fprintf(stderr, "message has no arugments!\n");
	else if (DBUS_TYPE_UINT32 != (arg=dbus_message_iter_get_arg_type(&args2))) {
		fprintf(stderr, "Argument is not int!\n");
		switch(arg){
			case DBUS_TYPE_STRING:
				char *string;
				dbus_message_iter_get_basic(&args2, &string);
				fprintf(stderr, "ITS A STRING: %s\n", string);
				break;
	}}
	else { 
		dbus_message_iter_get_basic(&args2, &id);
		printf("ID: %d\n", id);
	}

	dbus_message_unref(message);
	dbus_connection_close(conn);

    return 0;
}

