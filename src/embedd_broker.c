#include <mosquitto/embedded_broker.h>
#include "mosquitto_broker_internal.h"

extern int run;

void mosquitto_broker_stop() {
    /* Just stop gracefully. */
    run = 0;
}
