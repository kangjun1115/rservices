#ifndef ABSTRACT_SERVICE_H_
#define ABSTRACT_SERVICE_H_

#include "rservice.h"
#include "message.h"
#include "abstract_message_queue.h"
#include "abstract_thread.h"
#include "error_category.h"

class AbstractService {
public:
	AbstractService(AbstractThread& thread, AbstractMessageQueue& message_queue) :
		thread_(thread),
		message_queue_(message_queue) { }
	virtual ~AbstractService() { }

	virtual void Work() = 0;
	virtual void Start(const bool one_shot = false) {
		message_queue_.EmptyDownStream();
		thread_.SetWork([&](){ this->Work(); });
		thread_.Start(one_shot);
	}
	virtual void Stop() {
		thread_.Stop();
	}
	virtual error_condition SendMessage(const Message& message) {
		return message_queue_.Send(message);
	}
	virtual error_condition ReceiveMessage(Message& message) {
		return message_queue_.Receive(message);
	}
	virtual string GetMessageQueueName() {
		return message_queue_.GetName();
	}

private:
	AbstractThread& thread_;
	AbstractMessageQueue& message_queue_;
};

#endif /* ABSTRACT_SERVICE_H_ */
