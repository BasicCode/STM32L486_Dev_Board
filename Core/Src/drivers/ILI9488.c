/*
 * LCD driver for the ILI9488 TFT LCD chips.
 *
 * Feel free to use, change, distribute this code as desired. Use under
 * GPLv3 open-source license.
 *
 * File:   ILI9488.c
 * Author: tommy
 *
 * Created on 29th March 2021
 */


#include "drivers/ILI9488.h"
#include "font.h"
#include "bigfont/bigfont.h"

/**
 * Writes data to an 16-bit parallel bus.
 */
void parallel_write(unsigned int data) {
	//Manual, dangerous, GPIO write here because HAL is slow
	WR_PORT->ODR = (~WR_PIN & WR_PORT->ODR);
    //HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_RESET);

	//In this particular example I'm using PA8:15 and PC8:15
	GPIOC->ODR = (data & 0xFF00) | (GPIOC->ODR & 0x00FF); //Remember, we're using the lower half of these pins for other things.
	GPIOA->ODR = ((data << 8)) | (GPIOA->ODR & 0x00FF);

	//
    //HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_SET);
	WR_PORT->ODR = (WR_PIN | WR_PORT->ODR);
}

/*
 * Writes a data byte to the display. Pulls CS low as required.
 */
void lcd_write_data(unsigned int data) {
    //HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
	//Manual low level port write for speed
	DC_PORT->ODR = (DC_PIN | DC_PORT->ODR);
	CS_PORT->ODR = (~CS_PIN & CS_PORT->ODR);
	parallel_write(data);
	CS_PORT->ODR = (CS_PIN | CS_PORT->ODR);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

/*
 * Writes a command byte to the display
 */
void lcd_write_command(unsigned char data) {
    //HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
	//Manual low level port write for speed
	DC_PORT->ODR = (~DC_PIN & DC_PORT->ODR);
	CS_PORT->ODR = (~CS_PIN & CS_PORT->ODR);
	parallel_write(data);
	CS_PORT->ODR = (CS_PIN | CS_PORT->ODR);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void lcd_write_reg(unsigned int data) {
    //HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
	//Manual low level port write for speed
	DC_PORT->ODR = (DC_PIN | DC_PORT->ODR);
	CS_PORT->ODR = (~CS_PIN & CS_PORT->ODR);
	parallel_write(data);
	CS_PORT->ODR = (CS_PIN | CS_PORT->ODR);
    //HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

/*
 * Swaps two 16-bit integers
 */
void swap_int(unsigned int *num1, unsigned int *num2) {
    int temp = *num2;
    *num2 = *num1;
    *num1 = temp;
}

/*
 * Swaps two 8-bit integers
 */
void swap_char(char *num1, char *num2) {
    char temp = *num2;
    *num2 = *num1;
    *num1 = temp;
}

/*
 * Delay calcualted on 32MHz clock.
 * Does NOT adjust to clock setting
 */
void delay_ms(double millis) {
    int multiplier = 4;
    double counter = millis;
    while(multiplier--) {
        while(counter--);
        counter = millis;
    }
}

/*
 * A short microsecond delay routine
 * (not measured)
 */
void delay_us(long int cycles) {
    while(cycles--);
}

/*
 * Initialisation routine for the LCD
 * I got this from the one of the ebay sellers which make them.
 * From Open-Smart
 */
void lcd_init_parallel() {

    //SET control pins for the LCD HIGH (they are active LOW)
    HAL_GPIO_WritePin(RESX_PORT, RESX_PIN, GPIO_PIN_SET); //RESET pin HIGH (Active LOW)
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET); //Chip Select Active LOW
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET); //Data / Command select Active LOW
    HAL_GPIO_WritePin(RD_PORT, RD_PIN, GPIO_PIN_SET); //READ pin HIGH (active LOW)
    HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_SET); //WRITE pin HIGH (active LOW)
    //Cycle reset pin
    delay_ms(100);
    HAL_GPIO_WritePin(RESX_PORT, RESX_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RESX_PORT, RESX_PIN, GPIO_PIN_SET);
    HAL_Delay(100);

    lcd_init_command_list();

}

/**
 * This is the magic initialisation routine.
 */
void lcd_init_command_list(void)
{

	//********Start Initial Sequence*******//
	lcd_write_command(0xE0); //P-Gamma
	lcd_write_data(0x00);
	lcd_write_data(0x13);
	lcd_write_data(0x18);
	lcd_write_data(0x04);
	lcd_write_data(0x0F);
	lcd_write_data(0x06);
	lcd_write_data(0x3A);
	lcd_write_data(0x56);
	lcd_write_data(0x4D);
	lcd_write_data(0x03);
	lcd_write_data(0x0A);
	lcd_write_data(0x06);
	lcd_write_data(0x30);
	lcd_write_data(0x3E);
	lcd_write_data(0x0F);
	lcd_write_command(0XE1); //N-Gamma
	lcd_write_data(0x00);
	lcd_write_data(0x13);
	lcd_write_data(0x18);
	lcd_write_data(0x01);
	lcd_write_data(0x11);
	lcd_write_data(0x06);
	lcd_write_data(0x38);
	lcd_write_data(0x34);
	lcd_write_data(0x4D);
	lcd_write_data(0x06);
	lcd_write_data(0x0D);
	lcd_write_data(0x0B);
	lcd_write_data(0x31);
	lcd_write_data(0x37);
	lcd_write_data(0x0F);
	lcd_write_command(0xC0);
	lcd_write_data(0x18);
	lcd_write_data(0x16);
	lcd_write_command(0xC1);
	lcd_write_data(0x45);
	lcd_write_command(0xC5); //VCOM
	lcd_write_data(0x00);
	lcd_write_data(0x63);
	lcd_write_data(0x01);

	lcd_write_command(0x36); //RAM address mode
	//0xF8 and 0x3C are landscape mode. 0x5C and 0x9C for portrait mode.
	if(LANDSCAPE)
		lcd_write_data(0xF8);
	else
		lcd_write_data(0x5C);

	lcd_write_command(0x3A); //Interface Mode Control
	lcd_write_data(0x55); //x55 for 16-bit mode OR 0x66 for 18-bit serial mode
	lcd_write_command(0xB0); //Interface Mode Control
	lcd_write_data(0x80); //SDO not in use
	lcd_write_command(0xB1); //Frame rate 70HZ
	lcd_write_data(0x00); //
	lcd_write_data(0x10);
	lcd_write_command(0xB4);
	lcd_write_data(0x02);

	lcd_write_command(0xB6); //RGB/MCU Interface Control
	lcd_write_data(0x02);
	//lcd_write_data(0x22);

	lcd_write_command(0xE9);
	lcd_write_data(0x00);
	lcd_write_command(0xF7);
	lcd_write_data(0xA9);
	lcd_write_data(0x51);
	lcd_write_data(0x2C);
	lcd_write_data(0x82);
	lcd_write_command(0x11);
	HAL_Delay(120);
	lcd_write_command(0x21);


	HAL_Delay(120);
	lcd_write_command(0x29);
}

/*
 * Sets the X,Y position for following commands on the display.
 * Should only be called within a function that draws something
 * to the display.
 */
void set_draw_window(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {

    //Check that the values are in order
    if(x2 < x1)
        swap_int(&x2, &x1);
    if(y2 < y1)
        swap_int(&y2, &y1);

    lcd_write_command(ILI9488_CASET);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1 & 0xFF);

    lcd_write_data(x2 >> 8);
    lcd_write_data(x2 & 0xFF);

    lcd_write_command(ILI9488_PASET);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1 & 0xFF);

    lcd_write_data(y2 >> 8);
    lcd_write_data(y2 & 0xFF);

    lcd_write_command(ILI9488_RAMWR);
}

/*
 * Draws a single pixel to the LCD at position X, Y, with
 * Colour.
 *
 * 28 bytes per pixel. Use it wisely.
 */
void draw_pixel(unsigned int x, unsigned int y, unsigned int colour) {

    //Set the x, y position that we want to write to
    set_draw_window(x, y, x+1, y+1);
    lcd_write_data(colour);
}

/*
 * Fills a rectangle with a given colour
 */
void fill_rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int colour) {


    //Set the drawing region
    set_draw_window(x1, y1, x2, y2);

    // data sections but I don't trust it.)
    //CS low to begin data
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);


    //Write colour to each pixel
    for(int y = 0; y < y2-y1+1 ; y++) {
        for(int x = 0; x < x2-x1+1; x++) {
        	parallel_write(colour);
        }
    }

    //Return CS to high
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

/*
 * draws an empty rectangle with a given border weight.
 */
void empty_rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int colour, unsigned int border_weight) {

	//Top border
	fill_rectangle(x1 + border_weight, y1, x2, y1 + 1, colour);
	//Left border
	fill_rectangle(x1, y1 + border_weight, x1 + 1, y2, colour);
	//Bottom border
	fill_rectangle(x1 + border_weight, y2, x2, y2 + border_weight, colour);
	//Right border
	fill_rectangle(x2, y1 + border_weight, x2 + border_weight, y2, colour);
}

/*
 * Draws a single char to the screen.
 */
void draw_char(unsigned int x, unsigned int y, char c, unsigned int colour, char size) {
    int i, j;
    char line;
    unsigned int font_index = (c - 32);

    //Get the line of pixels from the font file
    for(i=0; i<13; i++ ) {

        line = FontLarge[font_index][12 - i];

        //Draw the pixels to screen
		for(j=0; j<8; j++) {
			if(line & (0x01 << j)) {
				if(size == 1) {
					//If we are just doing the smallest size font then do a single pixel each
					draw_pixel(x+(8-j), y+i, colour);
				}
				else {
					// do a small box to represent each pixel
					fill_rectangle(x+((8-j)*size), y+((i)*size), x+((8-j)*size)+size, y+((i)*size)+size, colour);
				}
			}
		}
    }
}

/*
 * Draws a char to the screen using a constant stream of pixel data whic his faster
 * than drawing individual pixels.
 * This will draw over any background image though.
 *
 * NOTE: This sends 130 bytes for a regular sized char
 */
void draw_fast_char(unsigned int x, unsigned int y, char c, unsigned int colour, unsigned int bg_colour) {
    char line;
    char width = 8;
    char height = 13;
    unsigned int font_index = (c - 32);
    unsigned int this_px = bg_colour;

    //Set the drawing region
    set_draw_window(x, y, x + width - 1, y + height);

    //We will do the SPI write manually here for speed
    //CS low to begin data
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

    //Get the line of pixels from the font file
    for(int i=0; i < height; i++ ) {
        line = FontLarge[font_index][12 - i];

        //Draw the pixels to screen
        for(int j = width-1; j >= 0; j--) {
            //Default pixel colour is the background colour, unless changed below
            this_px = bg_colour;
			if((line >> (j)) & 0x01)
				this_px = colour;
			parallel_write(this_px);
        }
    }

    //Return CS to high
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}


/*
 * Writes a string to the display as an array of chars at position x, y with
 * a given colour and size.
 */
void draw_string(unsigned int x, unsigned int y, unsigned int colour, char size, char *str) {

    //Work out the size of each character
    int char_width = size * 9;
    //Iterate through each character in the string
    int counter = 0;
    while(str[counter] != '\0') {
        //Calculate character position
        int char_pos = x + (counter * char_width);
        //Write char to the display
        draw_char(char_pos, y, str[counter], colour, size);
        //Next character
        counter++;
    }
}

/*
 * Draws a string using the draw_fast_char() function.
 * This will not preserve any background image and so a custom background
 * colour should be provided.
 * NOTE: Can only be the regular sized font. No scaling.
 */
void draw_fast_string(unsigned int x, unsigned int y, unsigned int colour, unsigned int bg_colour, char *str) {
    //Iterate through each character in the string
    int counter = 0;
    while(str[counter] != '\0') {
        //Write char to the display
        draw_fast_char(x + (counter * 9), y, str[counter], colour, bg_colour);
        //Next character
        counter++;
    }
}

/*
 * Draws a bitmap by directly writing the byte stream to the LCD.
 *
 * So the scaling is done strangely here because writing individual pixels
 * has an overhead of 26 bytes each.
 */
void draw_bitmap(unsigned int x1, unsigned int y1, int scale, const unsigned int *bmp) {
	int width = bmp[0];
	int height = bmp[1];
	unsigned int this_byte;
	int x2 = x1 + (width * scale);
	int y2 = y1 + (height * scale);

	//Set the drawing region
	set_draw_window(x1, y1, x2 + scale - 2, y2);

	//We will do the SPI write manually here for speed
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
	//CS low to begin data
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

	//Write colour to each pixel
	for (int y = 0; y < height; y++) {
		//this loop does the vertical axis scaling (two of each line))
		for (int sv = 0; sv < scale; sv++) {
			for (int x = 0; x < width; x++) {
				//Choose which byte to display depending on the screen orientation
				//NOTE: We add 2 bytes because of the first two bytes being dimension data in the array
				this_byte = bmp[(width * (y)) + x + 2];

				//And this loop does the horizontal axis scale (three bytes per pixel))
				for (int sh = 0; sh < scale; sh++) {
					parallel_write(this_byte);
				}
			}
		}
	}

	//Return CS to high
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

}

/**
 * Draws a bitmap, ignoring the specified transparency colour.
 *
 * NOTE: This uses a slow, pixel-by-pixel draw process.
 */
void draw_bitmap_with_alpha(unsigned int x1, unsigned int y1, unsigned int alphaColour, int scale, const unsigned int *bmp) {
	int width = bmp[0];
	int height = bmp[1];
	unsigned int this_byte;

	//Write colour to each pixel
	for (int y = 0; y < height; y++) {
		//this loop does the vertical axis scaling (two of each line))
		for (int sv = 0; sv < scale; sv++) {
			for (int x = 0; x < width; x++) {
				//Choose which byte to display depending on the screen orientation
				//NOTE: We add 2 bytes because of the first two bytes being dimension data in the array
				this_byte = bmp[(width * (y)) + x + 2];

				//And this loop does the horizontal axis scale (three bytes per pixel))
				for (int sh = 0; sh < scale; sh++) {
					if(this_byte != alphaColour )
						draw_pixel(x1 + (x * scale) + sh, y1 + (y * scale) + sv, this_byte);
				}
			}
		}
	}
}

/**
 * Fills a rectangle with a gradient between two colours.
 */
void fill_gradient(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int startColour, unsigned int endColour, Orientation orientation) {
	//Calculate each colour channel
	unsigned char rStart = (startColour >> 11) & 0x1F;
    unsigned char gStart = (startColour >> 5) & 0x3F;
    unsigned char bStart = (startColour) & 0x1F;
	unsigned char rEnd = (endColour >> 11) & 0x1F;
    unsigned char gEnd = (endColour >> 5) & 0x3F;
    unsigned char bEnd = (endColour) & 0x1F;
	//Number of incremental steps are we doing
    //Depends on orientation
    int steps = 0;
	if(orientation == HORIZONTAL)
		steps = (x2 - x1);
    if(orientation == VERTICAL)
    	steps = (y2 - y1);

	//Calculate difference between each colour channel
	float dR = ((float)(rEnd - rStart) / steps);
	float dG = ((float)(gEnd - gStart) / steps);
	float dB = ((float)(bEnd - bStart) / steps);

	//Figure out how often to change the colour
	unsigned int thisR = rStart;
	unsigned int thisG = gStart;
	unsigned int thisB = bStart;

    //Set the drawing region
    set_draw_window(x1, y1, x2, y2);

    //CS low to begin data
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);


	//Doing a normal rectangle fill but we change the colour as we go
    //Write colour to each pixel
    unsigned int colour = startColour;
    for(int y = 0; y <= y2-y1; y++) {
        for(int x = 0; x <= x2-x1; x++) {
        	//For a horizontal gradient, update on each X increment
            if(orientation == HORIZONTAL) {
    			//Increment the colours
    			thisR = rStart + (int)(dR * x);
    			thisG = gStart + (int)(dG * x);
    			thisB = bStart + (int)(dB * x);
    			//Combine them to RGB565
    			colour = (thisR << 11) | (thisG << 5) | (thisB);
            }

        	//Send them to the display
        	parallel_write(colour);
        }

        //For a vertical gradient, change on each Y increment
        if(orientation == VERTICAL) {
			//Increment the colours
			thisR = rStart + (int)(dR * y);
			thisG = gStart + (int)(dG * y);
			thisB = bStart + (int)(dB * y);
			//Combine them to RGB565
			colour = (thisR << 11) | (thisG << 5) | (thisB);
        }
    }

    //Return CS to high
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}
