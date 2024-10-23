/**
 * @file can.h
 * @brief CAN Bus functionality for ATMega162 via a MCP2515 CAN controller
 */

#ifndef CAN_H
#define CAN_H

#include "mcp2515.h"

/** @enum CanFrameType
 *  @brief CAN Bus message types
 */
typedef enum{
	FRAMETYPE_NONE = 0x00,
	FRAMETYPE_DATA = 0x00,
	FRAMETYPE_REMOTE  = 0x01,
	FRAMETYPE_ERROR = 0x02,
	FRAMETYPE_OVERLOAD  = 0x04,
} CanFrameType;

/** @struct CanRecvBuffer
 *  @brief A data buffer for containing incoming CAN Bus message data
 */
typedef struct{
	uint8_t data[64];
	CanFrameType frameType;
} CanRecvBuffer;

/** @struct Can_Msg
 *  @brief Can Bus Message with ID, length and data fields
 */
typedef struct{
	uint16_t id;
	uint8_t len;
	uint8_t data[8];
} CanMsg;

/*
* Initialize CAN controller via SPI
*/
void can_init(void);

#endif /* CAN_H */