/*  Function declarations for use in BlobRunner project
 *  by Max Körlinge
 */

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

/* Game logic functions
 * gamelogic.c
 */
void clock_init();
void clock_check();
void game_clock_tick();
void send_block(int lane);
void write_blob();
void page_scroll(int pagenr);
void scroll_playingfield();
void generate_obstacles();
void check_gameover();
void set_difficulty();
void move_player();
void start_screen();
void game_over();
void level_one();

//extern int* BLOCKS_COUNTERS[4]; 

/*  Delay assembly function from lab1
 *  delay.S
 */
void delay(int);    //insert milliseconds
