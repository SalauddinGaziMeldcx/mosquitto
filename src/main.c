
int mosquitto_broker_main(int argc, char *argv[], int (*pw_callback)(char* buf, int size, int rwflag, void* userdata));

int main(int argc, char *argv[], int (*pw_callback)(char* buf, int size, int rwflag, void* userdata))
{
  return mosquitto_broker_main(argc, argv, pw_callback);
}
