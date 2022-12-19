#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "zmq.h"

#define TOPIC "hello"
void *subscribe(void * arg)
{
	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket(context, ZMQ_SUB);

	zmq_bind(subscriber, "tcp://*:12345");   
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, TOPIC, strlen(TOPIC));
	printf("Sub topic: %s\n", TOPIC);

	char topic[256] = {0};
	char payload[1024] = {0};

	while(1) {
		memset(topic, 0, sizeof(topic));
		memset(payload, 0, sizeof(payload));

		int size = zmq_recv(subscriber, topic, sizeof(topic), 0);
		if (size == -1) {
			printf("recv topic error!!\n");
		}
		size = zmq_recv(subscriber, payload, sizeof(payload), 0);
		if (size == -1) {
			printf("recv payload error!!\n");
		}
		printf("Topic:%s  Msg:%s\n",topic, payload);
	}
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	return 0;
}

#define LOAD "world"

void * publish(void * arg)
{
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUB);

	zmq_connect(publisher, "tcp://localhost:12345");
	while(1) {
		zmq_send (publisher, TOPIC, strlen(TOPIC), ZMQ_SNDMORE);
		zmq_send (publisher, LOAD, strlen(LOAD), 0);
		//sleep(1);
	}

	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}

int main(int argc, char *argv[])
{
	pthread_t pub, sub;
	int ret;

	ret = pthread_create(&pub, NULL, subscribe, NULL);
	if (ret)
		return -1;

	publish(NULL);
	pthread_join(sub, NULL);
	return 0;
}
