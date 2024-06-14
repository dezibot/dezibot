#ifndef IMU_CMDs
#define IMU_CMDs

#define CMD_READ       0x80
#define CMD_WRITE      0x00
#define ADDR_MASK      0x7F

//Registers
#define MCLK_RDY    0x00

#define REG_TEMP_LOW   0x0A
#define REG_TEMP_HIGH  0X09

#define ACCEL_DATA_X_HIGH  0x0B
#define ACCEL_DATA_X_LOW   0x0C
#define ACCEL_DATA_Y_HIGH  0x0D
#define ACCEL_DATA_Y_LOW   0x0E
#define ACCEL_DATA_Z_HIGH  0x0F
#define ACCEL_DATA_Z_LOW   0x10

#define GYRO_DATA_X_HIGH   0x11
#define GYRO_DATA_X_LOW    0x12
#define GYRO_DATA_Y_HIGH   0x13
#define GYRO_DATA_Y_LOW    0x14
#define GYRO_DATA_Z_HIGH   0x15
#define GYRO_DATA_Z_LOW    0x16

#define PWR_MGMT0          0x1F
#define WHO_AM_I           0x75

#define INTF_CONFIG0       0x35

#define BLK_SEL_W          0x79
#define BLK_SEL_R          0x7C
#define MADDR_W            0x7A
#define MADDR_R            0x7D
#define M_W                0x7B
#define M_R                0x7E

#define FIFO_COUNTH        0x3D
#define FIFO_COUNTL        0x3E
#define FIFO_DATA          0x3F
#define FIFO_CONFIG1       0x28
#define FIFO_CONFIG2       0x29

//MREG1
#define FIFO_CONFIG5       0x01
#define TMST_CONFIG1       0x00
#endif