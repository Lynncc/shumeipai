#include "lewei.h"
#include <wiringPi.h>
#include "dht11.h"

static char * __VERSION = "0.0.2";

int main(int argc, char const *argv[])
{
	int sensor_socket;
	int switch_socket;
	int i;
	DHT11 dht11_data;

	while(wiringPiSetup() == -1)
		sleep(5);

INIT:
	printf("[Lewei] VERSION : %s\n", __VERSION);

	sleep(1);

	sensor_socket = server_connect(HOST_NAME, PORT);
	if (sensor_socket == -1)
		goto INIT;

	if(server_ping(sensor_socket, 1) != 0)
		goto INIT;

	while(1)
	{

		for (i = 0; i < 5; i++)
		{
			if (dht11_read_val(&dht11_data) == 1)
			{
				if(update_sensor(sensor_socket, "Temp", dht11_data.Temp) != 0)
					goto INIT;

				sleep(1);
				if(update_sensor(sensor_socket, "Hum", dht11_data.Hum) != 0)
					goto INIT;

				sleep(30);
				break;
			}
			sleep(1);
		}
		if (i >= 5)
		{
			if(server_ping(sensor_socket, 1) != 0)
				goto INIT;
			sleep(30);
		}

	}
	return 0;
}










