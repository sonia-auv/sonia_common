# SLAVE ID

uint8 SLAVE_PSU0 = 0
uint8 SLAVE_PSU1 = 1
uint8 SLAVE_PSU2 = 2
uint8 SLAVE_PSU3 = 3
uint8 SLAVE_KILLMISSION = 4
uint8 SLAVE_ESC = 5
uint8 SLAVE_IO = 6
uint8 SLAVE_STATE_SCREEN = 7
uint8 SLAVE_PWR_MANAGEMENT = 8

#CMD

# les define de la kill mission switch
uint8 CMD_MISSION = 0
uint8 CMD_KILL = 1

# les define de la backplane/PSU
uint8 CMD_VOLTAGE = 0
uint8 CMD_CURRENT = 1
uint8 CMD_TEMPERATURE = 2

# les define de la backplane/ESC
uint8 CMD_READ_MOTOR = 15
uint8 CMD_ACT_MOTOR = 16
uint8 CMD_PWM = 17

# define IO CONTROL
uint8 CMD_IO_TEMP= 0
uint8 CMD_IO_DROPPER_ACTION = 1
uint8 CMD_IO_TORPEDO_ACTION = 2
uint8 CMD_IO_ARM_ACTION = 3
uint8 CMD_IO_LEAK_SENSOR = 4

#Common CMD
# 30 is the last command number allowed
uint8 CMD_KEEP_ALIVE = 30

# DATA

# define IO CONTROL

uint8 DATA_IO_COULD_NOT_COMPLETE = 255

uint8 DATA_IO_DROPPER_PORT = 0
uint8 DATA_IO_DROPPER_STARBOARD = 1

uint8 DATA_IO_TORPEDO_PORT = 0
uint8 DATA_IO_TORPEDO_STARBOARD = 1

uint8 DATA_IO_ARM_CLOSE = 0
uint8 DATA_IO_ARM_OPEN = 1

uint8 DATA_IO_LEAK_SENSOR_DRY = 0
uint8 DATA_IO_LEAK_SENSOR_LEAK = 1

# SERVICE DEFINITION

uint8 slave
uint8 cmd
uint8[] data