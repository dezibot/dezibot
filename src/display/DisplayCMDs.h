#define cmd_byte 0x80
#define data_byte 0x40
#define muxRatio 0xa8 //needs second argument ranging from 16-63
#define setOffset 0xd3 //needs second argument ranging from 0-63
#define setStartLine 0x40
#define setSegmentMap 0xa0
#define setSegmentReMap 0xa1
#define setComDirectionNormal 0xc0
#define setComDirectionFlipped 0xc8
#define setComHardwareConfig 0xda //needs second arg
#define setContrast 0x81 //needs second arg
#define completeOn 0xa5
#define stopCompleteOn 0xa4
#define setNormalMode 0xa6
#define setInverseMode 0xa7
#define setOscFreq 0xd5 //needs second arg
#define setChargePump 0x8d//needs second arg 0x14 for enable
#define activateDisplay 0xaf
#define disableDisplay 0xae  
#define addressingMode 0x20
#define colRange 0x21
#define pageRange 0x22

#define DisplayAdress 0x3C