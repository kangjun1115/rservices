/*
 * dummy_protocol.h
 *
 *  Created on: 2020��10��17��
 *      Author: duanning
 */

#ifndef DUMMY_PROTOCOL_H_
#define DUMMY_PROTOCOL_H_

#include <iostream>
#include "abstract_protocol.h"

class DummyProtocol: public AbstractProtocol {
public:
	DummyProtocol() {};
	~DummyProtocol() {};

	virtual error_condition PackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& packed_data) {
		packed_data = buffer;
		return kNoError;
	};
	virtual error_condition UnpackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& unpacked_data) {
		unpacked_data = buffer;
		return kNoError;
	};
private:
	virtual bool IsFrameComplete(const vector<uint8_t>& buffer) {return true;};
	virtual bool IsFrameValid(const vector<uint8_t>& buffer) {return true;};
	virtual error_condition CalcCheckCode(\
			const vector<uint8_t>& buffer, vector<uint8_t> check_code) {
		return kNoError;
	};
};



#endif /* COMMON_INCLUDE_DUMMY_PROTOCOL_H_ */
