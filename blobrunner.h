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
uint8_t spi2putbyte(uint8_t bytetowrite);


