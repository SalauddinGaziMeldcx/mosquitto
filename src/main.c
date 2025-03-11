
int mosquitto_broker_main(int argc, char* argv[], int sendBuffer, int receiveBuffer);

int main(int argc, char *argv[])
{
  return mosquitto_broker_main(argc, argv, 0, 0);
}
