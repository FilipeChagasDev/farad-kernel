//Code taken and adapted from https://github.com/LdB-ECM/Raspi3-Kernel

#include "gpio.h"

/*
 * This function is practically the same as the pinmode() function
 * on the Arduino you specify a pin number then a GPIO mode
 * the different modes are as follows:
 * GPIO_INPUT
 * GPIO_OUTPUT
 * GPIO_ALTFUNC0
 * GPIO_ALTFUNC1
 * GPIO_ALTFUNC2
 * GPIO_ALTFUNC3
 * GPIO_ALTFUNC4
 * GPIO_ALTFUNC5
 *
 * The mode with the largest binary value is ALTFUNC3. That is why 
 * we check is the mode is invalid by checking if the input is larger
 * than it
 *
 * A description of this function is as follows
 * one GPFSEL register controls multiple pins depending on what bits are written to 
 * in the register. 
 * 1. Get the address of the first GPFSEL register
 * 2. Check inputs to see is they are valid if they aren't return FALSE
 * 3. Create a bit mask to write to the gpio we want
 * 4. Read the register corralating with the gpio we want to set
 * 5. Clear GPIO mode bits for the pin we want to set
 * 6. Use a logical or to write to the read of the register
 * 7. Write the value from the previous instruction or the register
 * 8. Return TRUE
 */
boolean_t gpio_setup(uint_t gpio_pin, GPIOMODE mode)
{
	uint_t *GPFSEL = (uint_t *)(MMIO_BASE + 0x200000 + 0x0); //Get register address
	if(gpio_pin > 54 || mode < 0 || mode > GPIO_ALTFUNC3) return FALSE; //Check inputs
	uint_t bit_mask = ((gpio_pin % 10) * 3); //Create bit mask
	uint_t register_read = GPFSEL[gpio_pin / 10]; //Read the correct register
	register_read &= ~(7 << bit_mask); //Clear the mode bits of the pin we want to set
	register_read |= (mode << bit_mask); //Set the gpio mode
	GPFSEL[gpio_pin / 10] = register_read; // Write to the register
	return TRUE;
}

/*
 * gpio_init()
 * This function initializes the get_gpio_pin_status struct so it can be 
 * used
 * It doesn't have a return value and it has no inputs
 */
void gpio_init()
{
	gpio_pin_status.gpio_pin_1 = 0;
	gpio_pin_status.gpio_pin_2 = 0;
	gpio_pin_status.gpio_pin_3 = 0;
	gpio_pin_status.gpio_pin_4 = 0;
	gpio_pin_status.gpio_pin_5 = 0;
	gpio_pin_status.gpio_pin_6 = 0;
	gpio_pin_status.gpio_pin_7 = 0;
	gpio_pin_status.gpio_pin_8 = 0;
	gpio_pin_status.gpio_pin_9 = 0;
	gpio_pin_status.gpio_pin_10 = 0;
	gpio_pin_status.gpio_pin_11 = 0;
	gpio_pin_status.gpio_pin_12 = 0;
	gpio_pin_status.gpio_pin_13 = 0;
	gpio_pin_status.gpio_pin_14 = 0;
	gpio_pin_status.gpio_pin_15 = 0;
	gpio_pin_status.gpio_pin_16 = 0;
	gpio_pin_status.gpio_pin_17 = 0;
	gpio_pin_status.gpio_pin_18 = 0;
	gpio_pin_status.gpio_pin_19 = 0;
	gpio_pin_status.gpio_pin_20 = 0;
	gpio_pin_status.gpio_pin_21 = 0;
	gpio_pin_status.gpio_pin_22 = 0;
	gpio_pin_status.gpio_pin_23 = 0;
	gpio_pin_status.gpio_pin_24 = 0;
	gpio_pin_status.gpio_pin_25 = 0;
	gpio_pin_status.gpio_pin_26 = 0;
	gpio_pin_status.gpio_pin_27 = 0;
	gpio_pin_status.gpio_pin_28 = 0;
	gpio_pin_status.gpio_pin_29 = 0;
	gpio_pin_status.gpio_pin_30 = 0;
	gpio_pin_status.gpio_pin_31 = 0;
	gpio_pin_status.gpio_pin_32 = 0;
	gpio_pin_status.gpio_pin_33 = 0;
	gpio_pin_status.gpio_pin_34 = 0;
	gpio_pin_status.gpio_pin_35 = 0;
	gpio_pin_status.gpio_pin_36 = 0;
	gpio_pin_status.gpio_pin_37 = 0;
	gpio_pin_status.gpio_pin_38 = 0;
	gpio_pin_status.gpio_pin_39 = 0;
	gpio_pin_status.gpio_pin_40 = 0;
	gpio_pin_status.gpio_pin_41 = 0;
	gpio_pin_status.gpio_pin_42 = 0;
	gpio_pin_status.gpio_pin_43 = 0;
	gpio_pin_status.gpio_pin_44 = 0;
	gpio_pin_status.gpio_pin_45 = 0;
	gpio_pin_status.gpio_pin_46 = 0;
	gpio_pin_status.gpio_pin_47 = 0;
	gpio_pin_status.gpio_pin_48 = 0;
	gpio_pin_status.gpio_pin_49 = 0;
	gpio_pin_status.gpio_pin_50 = 0;
	gpio_pin_status.gpio_pin_51 = 0;
	gpio_pin_status.gpio_pin_52 = 0;
	gpio_pin_status.gpio_pin_53 = 0;
	gpio_pin_status.gpio_pin_54 = 0;
}

/*
 * get_gpio_pin_status(uint_t gpio_pin)
 * This has a boolean return value that shows the current status of
 * whether a pin is activated as an output or not it returns 2 if the pin
 * is currently an input. A 0 is returned if the pin is currently off and
 * A 1 is returned if the pin is on and giving 3.3v
 * The only input is the gpio pin
 */

//TODO write the above function and track GPIO modes

/*
 * gpio_input(uint_t gpio_pin)
 * This function returns whether a GPIO pin is receiving a voltage or not
 * it returns TRUE if it is and FALSE if it isn't
 * Only works if the pin mode is set to input
 *
 * The steps are as follows:
 * 1. Set the register address
 * 2. Check the inputs
 * 3. If the inputs are ok then do the following steps
 * 4. Create the bit mask
 * 5. Read the port level
 * 6. Return TRUE if the bit is set
 * 7. Return FALSE
 */

         //////////////////////////////TODO/////////////////////////
	 // Create a way of tracking the gpio mode in software and//
	 // throw an error if is isn't the right gpio mode        //
	 //////////////////////////////TODO/////////////////////////

boolean_t gpio_input(uint_t gpio_pin)
{
	uint_t *GPIO_LEVEL = (uint_t *)(MMIO_BASE + 0x200000 + 0x34);
	if(gpio_pin < 54) //The gpio_pin cannot be negative it is unsigned
	{
		uint_t bit_to_write = 1 << (gpio_pin % 32);
		uint_t register_read = GPIO_LEVEL[gpio_pin / 32];
		if(register_read & bit_to_write) return TRUE; //If the bit isn't set this evaluates to 0
	}
	return FALSE;
}

/*
 * gpio_output(uint_t gpio_pin, boolean_t on)
 * If the output mode is set on a pin you can send a 3.3v signal on that pin or turn a pin off
 * 
 * To acomplish this we have to:
 * 1. Check if the GPIO pin number is valid
 * 2. Create a temporary pointer
 * 3. Set which register we need to write to
 * 4. Create the bit mask
 * 5. If the output is being turned on then set the temp pointer to the GPSET0 register
 * 6. Else set the temp pointer GPCLR0 register
 * 7. Output the bitmask to the selected register
 * 8. Return TRUE on sucsess
 * 9. Return FALSE if the GPIO set failed
 */
boolean_t gpio_output(uint_t gpio_pin, boolean_t on)
{
	if(gpio_pin < 54)
	{
		volatile uint_t *clear_or_set_register_pointer;
		uint_t register_number = gpio_pin / 32;
		uint_t bit_to_write = 1 << (gpio_pin % 32);
		if(on)
		{
			clear_or_set_register_pointer = (uint_t *)(GPSET0);
		}
		else
		{
			clear_or_set_register_pointer = (uint_t *)(GPCLR0);
		}
		clear_or_set_register_pointer[register_number] = bit_to_write;
		return TRUE;
	}
	return FALSE;
}

/*
 * These following functions are just to make a mostly arduino compadible environment
 * [WARNING] These functions fail silently
 */

void pinMode(uint_t gpio_pin, GPIOMODE mode)
{
	gpio_setup(gpio_pin, mode);
}

void digitalWrite(uint_t gpio_pin, boolean_t value)
{
	gpio_output(gpio_pin, value);
}

boolean_t digitalRead(uint_t gpio_pin)
{
	return gpio_input(gpio_pin);
}
