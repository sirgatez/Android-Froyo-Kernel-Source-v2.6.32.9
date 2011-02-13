#ifndef __GP2A_H__
#define __GP2A_H__

#define I2C_M_WR 0 /* for i2c */
#define I2c_M_RD 1 /* for i2c */


//#define DELAY_PRX 1

#define IRQ_GP2A_INT IRQ_EINT2  /*s3c64xx int number */

#define I2C_DF_NOTIFY			0x01 /* for i2c */

//ADDSEL is LOW
#define GP2A_ADDR		0x88 /* slave addr for i2c */

#define REGS_PROX 	    0x0 // Read  Only
#define REGS_GAIN    	0x1 // Write Only
#define REGS_HYS		0x2 // Write Only
#define REGS_CYCLE		0x3 // Write Only
#define REGS_OPMOD		0x4 // Write Only
#if defined(CONFIG_GP2A_MODE_B)
#define REGS_CON	0x6 // Write Only
#endif

/* sensor type */
#define LIGHT           0
#define PROXIMITY		1
#define ALL				2

/* power control */
#define ON              1
#define OFF				0

/* IOCTL for proximity sensor */
#define SHARP_GP2AP_IOC_MAGIC   'C'                                 
#define SHARP_GP2AP_OPEN    _IO(SHARP_GP2AP_IOC_MAGIC,1)            
#define SHARP_GP2AP_CLOSE   _IO(SHARP_GP2AP_IOC_MAGIC,2)      

/* input device for proximity sensor */
#define USE_INPUT_DEVICE 	0  /* 0 : No Use  ,  1: Use  */


#define INT_CLEAR    1 /* 0 = by polling operation, 1 = by interrupt operation */
#define LIGHT_PERIOD 1 /* per sec */
#define ADC_CHANNEL  9 /* index for s5pC110 9번 channel adc */ 

/*for light sensor */
#define STATE_NUM			3   /* number of states */
#define LIGHT_LOW_LEVEL     1    /* brightness of lcd */
#define LIGHT_MID_LEVEL 	 11    
#define LIGHT_HIGH_LEVEL    23

#define ADC_BUFFER_NUM	6


#define GUARDBAND_BOTTOM_ADC    700
#define GUARDBAND_TOP_ADC    800




/*
 * STATE0 : 30 lux below
 * STATE1 : 31~ 3000 lux
 * STATE2 : 3000 lux over
 */




#define ADC_CUT_HIGH 1100            /* boundary line between STATE_0 and STATE_1 */
#define ADC_CUT_LOW  220            /* boundary line between STATE_1 and STATE_2 */
#define ADC_CUT_GAP  50            /* in order to prevent chattering condition */

#define LIGHT_FOR_16LEVEL 


/* 16 level for premium model*/
typedef enum t_light_state
{
	LIGHT_DIM   = 0,
	LIGHT_LEVEL1   = 1,
	LIGHT_LEVEL2   = 2,
	LIGHT_LEVEL3   = 3,
	LIGHT_LEVEL4   = 4,
	LIGHT_LEVEL5   = 5,
	LIGHT_LEVEL6   = 6,
	LIGHT_LEVEL7   = 7,
	LIGHT_LEVEL8   = 8,
	LIGHT_LEVEL9   = 9,
	LIGHT_LEVEL10   = 10,
	LIGHT_LEVEL11   = 11,
	LIGHT_LEVEL12   = 12,
	LIGHT_LEVEL13   = 13,	
	LIGHT_LEVEL14   = 14,		
	LIGHT_LEVEL15   = 15,	
	LIGHT_LEVEL16   = 16,	
	LIGHT_INIT  = 17,	
}state_type;

/* initial value for sensor register */
static u8 gp2a_original_image[8] =
{
#if defined(CONFIG_GP2A_MODE_B)
	0x00,	// REGS_PROX
	0x08,	// REGS_GAIN
	0x40,	// REGS_HYS
	0x04,	// REGS_CYCLE
	0x03,	// REGS_OPMOD
#else
	0x00,  
	0x08,  
	0xC2,  
	0x04,
	0x01,
#endif
};

/* for state transition */
struct _light_state {
	int adc_value;
	double lux;

};

/* Table of adc values and the coresponding lux value
   Table has the lowest adc value at the end, we walk through the
   Table from the back as we are more often in lower light then in 
   heig regions */
   
static struct _light_state light_state[] = {
[0] = {
.adc_value = 2500, //the sensor only delivers values up to 2100 this is to sure
.lux = 8000.0,
},
[1] = {
.adc_value = 1800, 
.lux = 6000.0,
},
[2] = {
.adc_value = 1730, 
.lux = 5000.0,
},
[3] = {
.adc_value = 1650, 
.lux = 4000.0,
},
[4] = {
.adc_value = 1600, 
.lux = 3000.0,
},
[5] = {
.adc_value = 1550, 
.lux = 2300.0,
},
[6] = {
.adc_value = 1500, 
.lux = 19000.0,
},
[7] = {
.adc_value = 1450, 
.lux = 1600.0,
},
[8] = {
.adc_value = 1400, 
.lux = 1300.0,
},
[9] = {
.adc_value = 1380, 
.lux = 1200.0,
},
[10] = {
.adc_value = 1360, 
.lux = 1100.0,
},
[11] = {
.adc_value = 1340, 
.lux = 1000.0,
},
[12] = {
.adc_value = 1320, 
.lux = 900.0,
},
[13] = {
.adc_value = 1300, 
.lux = 800.0,
},
[14] = {
.adc_value = 1250, 
.lux = 700.0,
},
[15] = {
.adc_value = 1200, 
.lux = 620.0,
},
[16] = {
.adc_value = 1150, 
.lux = 540.0,
},
[17] = {
.adc_value = 1100, 
.lux = 460.0,
},
[18] = {
.adc_value = 1050, 
.lux = 380.0,
},
[19] = {
.adc_value = 1000, 
.lux = 300.0,
},
[20] = {
.adc_value = 900, 
.lux = 260.0,
},
[21] = {
.adc_value = 800, 
.lux = 220.0,
},
[22] = {
.adc_value = 700, 
.lux = 180.0,
},
[23] = {
.adc_value = 600, 
.lux = 140.0,
},
[24] = {
.adc_value = 500, 
.lux = 100.0,
},
[25] = {
.adc_value = 150, 
.lux = 1.0,
},
}; 

#define LEVEL_COUNT 26 /*Count of the levels in the Table*/

/* driver data */
struct gp2a_data {
	struct input_dev *input_dev;
	struct work_struct work_prox;  /* for proximity sensor */
	struct work_struct work_light; /* for light_sensor     */
	int             irq;
    struct hrtimer timer;
	struct timer_list light_init_timer;

};


struct workqueue_struct *gp2a_wq;
#if defined(CONFIG_GP2A_MODE_B)
struct workqueue_struct *gp2a_wq_prox;
#endif

/* prototype */
extern short gp2a_get_proximity_value(void);
extern bool gp2a_get_lightsensor_status(void);
int opt_i2c_read(u8 reg, u8 *val, unsigned int len );
int opt_i2c_write( u8 reg, u8 *val );
extern int s3c_adc_get_adc_data(int channel);
void lightsensor_adjust_brightness(int level);
extern int lightsensor_backlight_level_ctrl(int value);
static int proximity_open(struct inode *ip, struct file *fp);
static int proximity_release(struct inode *ip, struct file *fp);
static int light_open(struct inode *ip, struct file *fp);
static int light_release(struct inode *ip, struct file *fp);

int lightsensor_get_adcvalue(void);


#endif
