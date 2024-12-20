#ifndef IR_H
#define IR_H

#include <sam.h>

/**
 * @brief Initialize the IR pin
 */
void IR_init(void);

/**
 * @brief Reads the IR pin
 * 
 * @return int 
 */
uint8_t getIR(void);

#endif /* IR_H */