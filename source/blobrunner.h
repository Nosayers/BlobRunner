/*  Function declarations for use in ProjectSnake project
 */

/*  Delay assembly function from lab1
 */
void delay(int);    //insert milliseconds

/* Display related functions (display.c)
 */

void display_hardware_init(void);
void display_controller_init(void);
void display_poweroff(void);
void display_update(void);
void display_putbuffer(int number_bytes, uint8_t* buffer_to_send);
uint8_t spi2putbyte(uint8_t bytetowrite);

extern uint8_t display_buffer[512];  //whats written in here gets output on display after a display_update()

extern uint8_t icon[512];
