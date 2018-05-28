#pragma once

#include <AltSoftSerial.h>

#define PKT_BEGIN 02
#define PKT_END 03

AltSoftSerial pkt_radio;

void pkt_initRadio() {
	pkt_radio.begin(9600);
}

uint8_t pkt_readByte() {
	uint8_t byte;
	pkt_radio.readBytes(&byte, 1);
	return byte;
}

#define HEADER_SIZE 3

enum PacketType {
	NONE = 0,
	CONTROL_INPUT = 1,
	CONTROL_ACK = 2
};

enum PktState {
	WAIT_HEADER,
	WAIT_PAYLOAD,
	PKT_RECIEVED
};

PktState pkt_state = WAIT_HEADER;

PacketType pkt_payloadType;
size_t pkt_payloadSize;
uint8_t pkt_payload[20];

bool pkt_available() {
	return (pkt_state == PKT_RECIEVED);
}

void pkt_update() {
	if (pkt_state == WAIT_HEADER) {
		if (pkt_radio.available() >= HEADER_SIZE) {
			uint8_t startByte = pkt_readByte();
			if (startByte != PKT_BEGIN) {
				return;
			}
			pkt_payloadType = (PacketType)pkt_readByte();
			pkt_payloadSize = (size_t)pkt_readByte();
			pkt_state = WAIT_PAYLOAD;
		}
	}
	if (pkt_state == WAIT_PAYLOAD) {
		//add one for the message end marker
		if (pkt_radio.available() >= pkt_payloadSize + 1) {
			for (int i = 0; i < pkt_payloadSize; i++) {
				uint8_t byte = pkt_readByte();
				pkt_payload[i] = byte;
			}
			//dispose of the end marker
			pkt_readByte();
			pkt_state = PKT_RECIEVED;
		}
	}
}

uint8_t* pkt_readPacket() { 
	pkt_state = WAIT_HEADER;
	return pkt_payload;
}

void pkt_send(PacketType type, uint8_t* data, size_t size) {
	pkt_radio.write((uint8_t)PKT_BEGIN);
	pkt_radio.write((uint8_t)type);
	pkt_radio.write((uint8_t)size);
	for (int i = 0; i < size; i++) {
		uint8_t byte = data[i];
		pkt_radio.write(byte);
	}
	pkt_radio.write((uint8_t)PKT_END);
}


//PACKET: CONTROL_INPUT
typedef struct _ControlInput {
	uint8_t pitch;
	uint8_t roll;
	uint8_t yaw;
	uint8_t throttle;
};

typedef union ControlInput {
	_ControlInput data;
	uint8_t bytes[sizeof(_ControlInput)];
};

void pkt_sendControlInput(ControlInput* joyInput) {
	size_t size = sizeof(_ControlInput);
	pkt_send(CONTROL_INPUT, joyInput->bytes, size);
}

ControlInput pkt_readControlInput() {
	uint8_t* payload = pkt_readPacket();
	ControlInput input;
	for (int i = 0; i < sizeof(ControlInput); i++) {
		input.bytes[i] = payload[i];
	}
	return input;
}

//PACKET: CONTROL_ACK

typedef struct _ControlAck {
	int voltage;
};

typedef union ControlAck {
	_ControlAck data;
	uint8_t bytes[sizeof(_ControlAck)];
};

void pkt_sendControlAck(ControlAck* ack) {
	size_t size = sizeof(_ControlAck);
	pkt_send(CONTROL_ACK, ack->bytes, size);
}

ControlAck pkt_readControlAck() {
	uint8_t* payload = pkt_readPacket();
	ControlAck ack;
	for (int i = 0; i < sizeof(ControlAck); i++) {
		ack.bytes[i] = payload[i];
	}
	return ack;
}

