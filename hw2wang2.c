/**************************************************
* CMPEN 473, Spring 2018, Penn State University
* 
* Homework 2 Sample Program 4
* Revision V1.1
* On 1/29/2020
* By Songmeng Wang
* 
***************************************************/

/* Homework 2 Sample Program 4
 * simple LED on/off program example in C for 
 * Raspberry Pi 3 computer with 
 * red   LED on GPIO18 (with 500 ohm resistor in series)
 * green LED on GPIO19 (with 500 ohm resistor in series)
 * 
 * 'r' => red   LED on/off, hit 'r' key to toggle red   LED on/off
 * 'g' => green LED on/off  hit 'g' key to toggle green LED on/off
 * hit any other key to quit
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>
#include "import_registers.h"
#include "gpio.h"
#include "cm.h"
#include "pwm.h"
#include "spi.h"
#include "io_peripherals.h"
#include "enable_pwm_clock.h"


int get_pressed_key(void)
{
  struct termios  original_attributes;
  struct termios  modified_attributes;
  int             ch;

  tcgetattr( STDIN_FILENO, &original_attributes );
  modified_attributes = original_attributes;
  modified_attributes.c_lflag &= ~(ICANON | ECHO);
  modified_attributes.c_cc[VMIN] = 1;
  modified_attributes.c_cc[VTIME] = 0;
  tcsetattr( STDIN_FILENO, TCSANOW, &modified_attributes );

  ch = getchar();

  tcsetattr( STDIN_FILENO, TCSANOW, &original_attributes );

  return ch;
}

int main( void )
{
  volatile struct io_peripherals *io;
  bool  done;

  io = import_registers();
  if (io != NULL)
  {
    /* print where the I/O memory was actually mapped to */
    printf( "mem at 0x%8.8X\n", (unsigned int)io );

    /* set the pin function to OUTPUT for GPIO18 - red LED light   */
    /* set the pin function to OUTPUT for GPIO19 - green LED light */
    io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_INPUT;
    io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_INPUT;

    /* set initial output state - off */

    printf( "\n press 'r' to toggle the red LED\n");
    printf( " press 'g' to toggle the green LED\n");
    printf( " press 'b' to toggle the blue LED\n");
    printf( " press 'o' to toggle the orange LED\n");
    printf( " any other character will exit\n");

    do
    {
      switch (get_pressed_key())
      {
        case 'r':
          io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_INPUT;
          io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_OUTPUT;
          GPIO_CLR ( &(io->gpio), 18);
          break;

        case 'g':
          io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_INPUT;
          io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_OUTPUT;
          GPIO_SET( &(io->gpio), 18);
          break;

        case 'b':
          io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_INPUT;
          io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_OUTPUT;
          GPIO_CLR ( &(io->gpio), 22);
          break;

        case 'o':
          io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_INPUT;
          io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_OUTPUT;
          GPIO_SET( &(io->gpio), 22);
          break;

        default:
          done = true;
          break;
      }
    } while (!done);

    /* clean the GPIO pins */
    io->gpio.GPFSEL1.field.FSEL8 = GPFSEL_INPUT;
    io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_INPUT;

    printf( "\n Key hit is not 'r', 'g', 'b', 'o' key, now quiting ... \n");

  }
  else
  {
    ; /* warning message already issued */
  }

  return 0;
}
