/*This library is designed with the help of the following github repo
*https://github.com/vrano714/max30102-tutorial-raspberrypi/blob/master/max30102.py?fbclid=IwAR3IUTtQ3xgon1Q3it_weUbdpVsIukNfI4z598LRBmG6shSCopwAqqP8zuA
*Author: Ismael Elsharkawi 
*Date: 21/5/2021
*Version: 1.0
*/
#include "stm32l4xx_hal.h"
#include "max30102.h"

extern I2C_HandleTypeDef hi2c1;

//# i2c address-es
//# not required?
#define I2C_WRITE_ADDR  0xAE
#define I2C_READ_ADDR  0xAF

//# register address-es
#define REG_INTR_STATUS_1  0x00
#define REG_INTR_STATUS_2  0x01

#define REG_INTR_ENABLE_1  0x02
#define REG_INTR_ENABLE_2  0x03

#define REG_FIFO_WR_PTR  0x04
#define REG_OVF_COUNTER  0x05
#define REG_FIFO_RD_PTR  0x06
#define REG_FIFO_DATA  0x07
#define REG_FIFO_CONFIG  0x08

#define REG_MODE_CONFIG  0x09
#define REG_SPO2_CONFIG  0x0A
#define REG_LED1_PA  0x0C

#define REG_LED2_PA  0x0D
#define REG_PILOT_PA  0x10
#define REG_MULTI_LED_CTRL1  0x11
#define REG_MULTI_LED_CTRL2  0x12

#define REG_TEMP_INTR  0x1F
#define REG_TEMP_FRAC  0x20
#define REG_TEMP_CONFIG  0x21
#define REG_PROX_INT_THRESH  0x30
#define REG_REV_ID  0xFE
#define REG_PART_ID  0xFF

//# currently not used
#define MAX_BRIGHTNESS  255


//class MAX30102():
//    # by default, this assumes that physical pin 7 (GPIO 4) is used as interrupt
//    # by default, this assumes that the device is at 0x57 on channel 1
int channel=1, address=0x57, gpio_pin=7;
int reg_data;
int data_written;
void initialize(){
        //print("Channel: {0}, address: 0x{1:x}".format(channel, address))
        //self.address = address
        //self.channel = channel
        //self.bus = smbus.SMBus(self.channel)
        //self.interrupt = gpio_pin
        /*We have to take care that there is an interrupt pin used*/
        //# set gpio mode
        //GPIO.setmode(GPIO.BOARD)
        //GPIO.setup(self.interrupt, GPIO.IN)

        //self.reset()
        reset();

        sleep(1);  # wait 1 sec
        /*Might need to look in the documentation to see the correct syntax and library for sleep*/
        //# read & clear interrupt register (read 1 byte)
        HAL_I2C_Mem_Read(&hi2c1, address, REG_INTR_STATUS_1, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, 10);

        //reg_data = self.bus.read_i2c_block_data(self.address, REG_INTR_STATUS_1, 1)
        //# print("[SETUP] reset complete with interrupt register0: {0}".format(reg_data))
        setup();
        //# print("[SETUP] setup complete")
}
void shutdown(){
        data_written = 0x80;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_MODE_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_MODE_CONFIG, [0x80])
}
void reset(){
        data_written = 0x40;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_MODE_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_MODE_CONFIG, [0x40])
}
int led_mode=0x03;
void setup(){
        //"""
        //This will setup the device with the values written in sample Arduino code.
        //"""
        //# INTR setting
        //# 0xc0 : A_FULL_EN and PPG_RDY_EN = Interrupt will be triggered when
        //# fifo almost full & new fifo data ready
        data_written = 0xc0;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_INTR_ENABLE_1, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_INTR_ENABLE_1, [0xc0])
        data_written = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_INTR_ENABLE_2, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_INTR_ENABLE_2, [0x00])

        //# FIFO_WR_PTR[4:0]
        data_written = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_FIFO_WR_PTR, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_FIFO_WR_PTR, [0x00])
        //# OVF_COUNTER[4:0]
        data_written = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_OVF_COUNTER, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
       
        //self.bus.write_i2c_block_data(self.address, REG_OVF_COUNTER, [0x00])
        //# FIFO_RD_PTR[4:0]
        data_written = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_FIFO_RD_PTR, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_FIFO_RD_PTR, [0x00])

        //# 0b 0100 1111
        //# sample avg = 4, fifo rollover = false, fifo almost full = 17
        data_written = 0x4f;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_FIFO_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_FIFO_CONFIG, [0x4f])
        //# 0x02 for read-only, 0x03 for SpO2 mode, 0x07 multimode LED
        data_written = led_mode;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_MODE_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_MODE_CONFIG, [led_mode])

        //# 0b 0010 0111
        //# SPO2_ADC range = 4096nA, SPO2 sample rate = 100Hz, LED pulse-width = 411uS
        data_written = 0x27;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_SPO2_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_SPO2_CONFIG, [0x27])
        //# choose value for ~7mA for LED1
        data_written = 0x24;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_LED1_PA, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_LED1_PA, [0x24])
        //# choose value for ~7mA for LED2
        data_written = 0x24;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_LED2_PA, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);

        //self.bus.write_i2c_block_data(self.address, REG_LED2_PA, [0x24])
        //# choose value fro ~25mA for Pilot LED
        data_written = 0x7f;
        HAL_I2C_Mem_Write(&hi2c1, address, REG_PILOT_PA, I2C_MEMADD_SIZE_8BIT, &data_written , 1, 10);
        //self.bus.write_i2c_block_data(self.address, REG_PILOT_PA, [0x7f])
}
    //# this won't validate the arguments!
    //# use when changing the values from default
void set_config(reg, value){
    HAL_I2C_Mem_Write(&hi2c1, address, reg, I2C_MEMADD_SIZE_8BIT, &value , 1, 10);
    //self.bus.write_i2c_block_data(self.address, reg, value);
}
int read_fifo_red_led, read_fifo_ir_led;//instead of multiple return statements, need to be defined as externs in the main program
int d[6] = {0,0,0,0,0,0};
void read_fifo(){
    //"""
    //This function will read the data register.
    //"""
    //red_led = None
    //ir_led = None

    //# read 1 byte from registers (values are discarded)
    int reg_INTR1, reg_INTR2;
    HAL_I2C_Mem_Read(&hi2c1, address, REG_INTR_STATUS_1, I2C_MEMADD_SIZE_8BIT, &reg_INTR1, 1, 10);
    HAL_I2C_Mem_Read(&hi2c1, address, REG_INTR_STATUS_2, I2C_MEMADD_SIZE_8BIT, &reg_INTR2, 1, 10);
    //reg_INTR1 = self.bus.read_i2c_block_data(self.address, REG_INTR_STATUS_1, 1)
    //reg_INTR2 = self.bus.read_i2c_block_data(self.address, REG_INTR_STATUS_2, 1)

    //# read 6-byte data from the device
    HAL_I2C_Mem_Read(&hi2c1, address, REG_FIFO_DATA,
                I2C_MEMADD_SIZE_8BIT, d,
                6 , 25);
    //d = self.bus.read_i2c_block_data(self.address, REG_FIFO_DATA, 6)

    //# mask MSB [23:18]
    read_fifo_red_led = (d[0] << 16 | d[1] << 8 | d[2]) & 0x03FFFF;
    read_fifo_ir_led = (d[3] << 16 | d[4] << 8 | d[5]) & 0x03FFFF;
    //return red_led, ir_led
}
/*The code here is initialized with a 200 element buffer because the python code had append function which is not an option in C*/
int read_seq_red_buf[200]{};
int read_seq_ir_buf[200]={};
int amount=100;
void read_sequential(){
    //"""
    //This function will read the red-led and ir-led `amount` times.
    //This works as blocking function.
    //"""
    //red_buf = []
    //ir_buf = []
    int i;
    for(i=0;i<amount;i++){
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1){} /*There is an implicit assumption that GPIOA pin 5 is the interrupt line*/
        read_fifo();
        red_buf[i] = read_fifo_red_led;
        ir_buf[i] = read_fifo_ir_led;
        //red_buf.append(red)
        //ir_buf.append(ir)
    }
    //return red_buf, ir_buf
}