#include <stdint.h>
#include "blobrunner.h"


/* Level one design (v0.1)
 */
int level_one_counter = 0;
void level_one(void) {

    switch (level_one_counter) {
        case 0:
            send_block(1);
            break;
        case 15:
            send_block(0);
            break;
        case 19:
            send_block(3);
            break;
        case 35:
            send_block(2);
            break;
        case 47:
            send_block(3);
            break;
    }

    level_one_counter++;

    if (level_one_counter > 58) {
        level_one_counter = 0;
    }
}
