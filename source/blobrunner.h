/*  Function declarations for use in BlobRunner project
 */

/*  Delay assembly function from lab1
 *  delay.S
 */
void delay(int);    //insert milliseconds

/* Display related functions
 * display.c
 */
//initialization and data handling
void display_hardware_init(void);
void display_controller_init(void);
void display_poweroff(void);
void display_putbuffer(int number_bytes, uint8_t* buffer_to_send);
uint8_t spi2putbyte(uint8_t bytetowrite);

//actually doing stuff on the screen
void display_playing_field(void);
void ready_page_for_input(int page);
void display_clear(void);
void enable_scrolling(void);


void fill_pix(int pagenumber, int x); 

/* Declarations for graphic data units, these are generally sent into the
 * display buffer
 * graphicsdata.c
 */
extern const uint8_t const  clear[512];
extern uint8_t field_page0[128];
extern uint8_t field_page1[128];
extern uint8_t field_page2[128];
extern uint8_t field_page3[128];
extern uint8_t* field_pages[4];
