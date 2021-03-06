#ifndef NRTMESSAGEQUEUE_H_
#define NRTMESSAGEQUEUE_H_

#include <mqueue.h>
#include "rservice.h"
#include "abstract_message_queue.h"

class NRTMessageQueue: public AbstractMessageQueue {
public:
	const unsigned int kNormalPriority = 50;

	NRTMessageQueue(const string& name, const bool on_switch = false);
	virtual ~NRTMessageQueue();

	virtual error_condition Send(const Message& message);
	virtual error_condition Receive(Message& message);
	virtual error_condition GetReceivingMessageCount(unsigned int& count);
	virtual error_condition EmptyDownStream();

private:
	mqd_t up_stream_queue_descriptor_;
	mqd_t down_stream_queue_descriptor_;
};

#endif /* NRTMESSAGEQUEUE_H_ */
