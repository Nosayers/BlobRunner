/*  Function declarations for use in BlobRunner project
 *  by Max Körlinge
 */

/* Initializing hardware (thats not Display related, see below)
 * and main function with main game loop
 * main.c
 */
void clock_init(void);
void button_init(void);

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
void fill_col(int pagenumber, int column);

/* Game logic functions
 * gamelogic.c
 */
void clock_check(void);
void game_clock_tick(void);
void send_block(int lane);
void write_blob(void);
void remove_blob(void);
void page_scroll(int pagenr);
void scroll_playingfield(void);
void generate_obstacles(void);
void move_player(int dir);
void start_screen(void);
void game_over(void);
void set_speed(int speed);
void button_interrupt(void);

/*  Helper assembly functions
 *  assembler.S
 */
void delay(int);    //insert milliseconds
void enable_interrupts(void);

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
extern uint8_t blob[6];

/* Levels
 * levels.c
 */
void level_one(void);
