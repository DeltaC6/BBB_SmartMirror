#include "main.h"

int main(int argc, char **argv) {
    fprintf(stdout, "Initializing application\n");

    float humidity = 0.0f;
    float temperature = 0.0f;

    int result = dht_read(DHT11, 0, 20, &humidity, &temperature);
    fprintf(stdout, "Humidity: %0.3f Temperature: %0.3f\n", humidity, temperature);

    fprintf(stdout, "Application complete... EXITING.\n");
    return EXIT_SUCCESS;
}
