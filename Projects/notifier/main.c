#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool init(DBusError **error, DBusConnection **connection) {
	*error = malloc(sizeof(struct DBusError));
	if(*error == NULL)
		return 1;

	//initialise the errors
	dbus_error_init(*error);

	//connect to the bus
	//*connection = dbus_connection_open("unix:path=/tmp/dbus-KaouvYEtHP", *error);
	
	*connection = dbus_bus_get(DBUS_BUS_SESSION, *error);


	if(*connection == NULL)
		return 1;

	if(dbus_error_is_set(*error)){
		fprintf(stderr, "Connection Error (%s)\n", (*error)->message);
		dbus_error_free(*error);
		return 1;
	}
	
	
	dbus_uint32_t result;
	result = dbus_bus_request_name(*connection, "sk.electricjozin.hackerman", DBUS_NAME_FLAG_DO_NOT_QUEUE, *error);

	if(result != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		fprintf(stderr, "Not primary owner of name\n");	
		return 1;
	}

	/*if(!dbus_bus_start_service_by_name(*connection, "org.freedesktop.Notifications", 0, &result, *error)){
		fprintf(stderr, "Starting notifications service failed, %s\n", (*error)->message);
		dbus_error_free(*error);
		return 1;
	}
	
	switch (result) {
		case DBUS_START_REPLY_SUCCESS:
			printf("Start reply success\n");
			break;
		case DBUS_START_REPLY_ALREADY_RUNNING:
			printf("Service already running\n");
			break;
	}
	*/
	
	return 0;
}

void add_arg_basic(DBusMessageIter * const args,const int type, void * const data){
	int status_code = dbus_message_iter_append_basic(args, type, data);
	if(!status_code){
		fprintf(stderr, "Error while appending args\n");
		exit(1);
	}
}

bool add_arg_fixed(DBusMessageIter * const args, DBusMessageIter ** const subargs) {
	*subargs = malloc(sizeof(struct DBusMessageIter));
	if(*subargs == NULL)
		return 1;

	dbus_message_iter_open_container(args, DBUS_TYPE_ARRAY, "s", *subargs);
	//static const char *str = " ";
	//dbus_message_iter_append_basic(subargs, DBUS_TYPE_STRING, &str);
	int status_code = dbus_message_iter_close_container(args, *subargs);
	if(!status_code){
		fprintf(stderr, "Error while appending args\n");
		//exit(1);
	}

	return 0;
}
bool add_arg_dict(DBusMessageIter *args, DBusMessageIter ** const subargs) {
	*subargs = malloc(sizeof(struct DBusMessageIter));

	if(*subargs == NULL)
		return 1;

	dbus_message_iter_open_container(args, DBUS_TYPE_DICT_ENTRY, NULL, *subargs);
	
	int status_code = dbus_message_iter_close_container(args, *subargs);
	if(!status_code){
		fprintf(stderr, "Error while appending args\n");
		//exit(1);
	}

	return 0;
}


bool send_message(DBusError * const error, DBusConnection * const connection) {
	/*if(!dbus_connection_get_is_connected(connection) || !dbus_connection_get_is_authenticated(connection)) {
		fprintf(stderr, "Connection is fucked\n");
		return 1;
	}
	*/

	DBusMessageIter args, replyargs;
	DBusPendingCall *pending;

	DBusMessage *message = dbus_message_new_method_call(
		"org.freedesktop.Notifications",
		"/org/freedesktop/Notifications",
		"org.freedesktop.Notifications",
		"Notify"
	);

	if(message == NULL) {
		fprintf(stderr, "Message Null\n");
		return 1;
	}
	
	//append arguments to iterator
	
	dbus_message_iter_init_append(message, &args);

	char *app_name = "App Name";
	char *app_icon = "App Icon";
	char *summary = "Summary";
	char *body = "Body";
	dbus_uint32_t replaces_id = 0;
	dbus_int32_t expire_timeout = 0;

	add_arg_basic(&args, DBUS_TYPE_STRING, &app_name);
	add_arg_basic(&args, DBUS_TYPE_UINT32, &replaces_id);
	add_arg_basic(&args, DBUS_TYPE_STRING, &app_icon);
	add_arg_basic(&args, DBUS_TYPE_STRING, &summary);
	add_arg_basic(&args, DBUS_TYPE_STRING, &body);
	
	DBusMessageIter* subargs1, *subargs2;

	add_arg_fixed(&args, &subargs1);
	add_arg_dict(&args, &subargs2);
	add_arg_basic(&args, DBUS_TYPE_INT32, &expire_timeout);
	dbus_uint32_t serial;

	

	if(!dbus_connection_send_with_reply(connection, message, &pending, DBUS_TIMEOUT_INFINITE))
		fprintf(stderr, "Out of memory!\n");

	if(pending == NULL)
		fprintf(stderr, "Pending call null\n");
	


	dbus_connection_flush(connection);

	dbus_message_unref(message);
	dbus_pending_call_block(pending);

	message = dbus_pending_call_steal_reply(pending);

	if(message == NULL){
		fprintf(stderr, "Reply null\n");
	}
	dbus_pending_call_unref(pending);
	int arg;
	dbus_uint32_t id;
	if(!dbus_message_iter_init(message, &replyargs))
		fprintf(stderr, "message has no arugments!\n");
	else if (DBUS_TYPE_UINT32 != (arg=dbus_message_iter_get_arg_type(&replyargs))) {
		fprintf(stderr, "Argument is not int!\n");
		switch(arg){
			case DBUS_TYPE_STRING:
				char *string;
				dbus_message_iter_get_basic(&replyargs, &string);
				fprintf(stderr, "ITS A STRING: %s\n", string);
				break;
	}}
	else { 
		dbus_message_iter_get_basic(&replyargs, &id);
		printf("ID: %d\n", id);
	}

	dbus_message_unref(message);
	dbus_connection_close(connection);
	//free(subargs1);
	//free(subargs2);
	return 0;
}


int main(int argc, char **argv) {
	DBusError* error;
	DBusConnection *connection;
	
	if(init(&error, &connection)) {
		fprintf(stderr, "Init failed\n");
		return 1;
	}

	if(send_message(error, connection)) {
		fprintf(stderr, "Sending message failed\n");
		return 1;
	}
	
	//while(getchar() != 'K');
	free(error);
}
