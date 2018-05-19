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
	JOY_INPUT = 12
};

enum PktState {
	WAIT_HEADER,
	WAIT_PAYLOAD,
	PKT_RECIEVED
};

PktState pkt_state;

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
				Serial.println(byte);
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


//PACKET: JOY_INPUT
enum JoyInputType {
	AXIS_X,
	AXIS_Y,
	CLICK = 66
};

typedef struct _JoyInput {
	JoyInputType type;
	unsigned long value;
};

typedef union JoyInput {
	_JoyInput data;
	uint8_t bytes[sizeof(_JoyInput)];
};

void pkt_sendJoyInput(JoyInput* joyInput) {
	size_t size = sizeof(_JoyInput);
	pkt_send(JOY_INPUT, joyInput->bytes, size);
}

JoyInput pkt_readJoyInput() {
	uint8_t* payload = pkt_readPacket();
	JoyInput input;
	for (int i = 0; i < sizeof(JoyInput); i++) {
		input.bytes[i] = payload[i];
	}
	return input;
}

