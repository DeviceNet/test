#ifndef	__MYTYPES_H__
#define	__MYTYPES_H__
/**
 basic type definitions
 */

#include <stdint.h>

typedef unsigned char byte;
typedef unsigned int word;

typedef unsigned int size_t;

#define	UINT8	uint8
#define	UINT16	uint16

#define bool unsigned char
#define true 1
#define false 0

// RF 상태
#define RF_STATE_INIT				0 // RF 초기화 상태
#define RF_STATE_SID_OK				1 // Sub ID 확인됨
#define RF_STATE_NID_OK				2 // Net ID 확인됨
#define RF_STATE_BAND_OK			3 // BAND 체크 완료
#define RF_STATE_CHANNEL_OK			4 // Channel 체크완료
#define RF_STATE_DID_OK				6 // DID 체크 완료
#define RF_STATE_DONE				7 // FOWARD 체크 완료
#define RF_STATE_RSSI_OK			8 // FOWARD 체크 완료
/**
 Define number of timers to use.
 */

#define TIMER_ID_WORK_COUNT 		0
#define TIMER_ID_ZS_SEND			1
#define TIMER_ID_NCP_WORK			2
#define TIMER_ID_SEND_WORK			3
#define TIMER_ID_LED_TASK			4
#define TIMER_ID_ADC_TASK			5
#define TIMER_ID_RF_DATA_EXPIRE		6
#define TIMER_ID_NETWORKID_EXPIRE	7
#define TIMER_ID_PASS_RF_GARBAGE	8
#define TIMER_ID_RF_SETTING			12
#define TIMER_ID_RF_RX_WAIT			13
#define TIMER_ID_RF_CHECK_RSSI		14

#define TIMER_INTERVAL_RF_SETTING_OK		180000UL
#define TIMER_INTERVAL_RF_DATA_EXPIRE		300000UL
#define TIMER_INTERVAL_NETWORKID_EXPIRE		300000UL

#define SENDING_STATE_READY		0
#define SENDING_STATE_SENDING	1

#define NETWORK_STATE_NOT_CONNECT		0
#define NETWORK_STATE_CONNECTED			1

#endif	//__TYPES_H__
