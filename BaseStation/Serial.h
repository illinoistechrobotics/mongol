/**
 * \file Serial.h
 *
 * \brief Serial communications interface header file.
 *
 * \details This header file contains the necessary includes for serial port
 * use and declares publicly available methods that may be used for communication with
 * the robot. It also defines some values that are intended to help build
 * serial communications packets.
 */

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define	PKT_BND	0xFF        /**< Value to fill the front_bnd and end_bnd fields
                              of packet with */
#define VAL_NUL 0x55        /**< Value filler for PKT_HELLO, PKT_STDBY,
                              PKT_GDBY, and PKT_RDY packets */

/**
 * \brief Serial communication intialization function.
 */
int init_serial( char* port );

/**
 * \brief Read-from-serial function.
 */
packet* read_serial( void );

/**
 * \brief Write-to-serial function.
 */
int write_serial( packet* msg );

/**
 * \brief Serial communication termination function.
 */
void close_serial( void );
