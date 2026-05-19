#pragma config FOSC = INTIO67   
#pragma config PLLCFG = OFF    
#pragma config PRICLKEN = ON    
#pragma config WDTEN = OFF      
#pragma config PWRTEN = OFF     
#pragma config BOREN = OFF      
#pragma config MCLRE = EXTMCLR  
#pragma config PBADEN = OFF    

#define _XTAL_FREQ 48000000UL
#include <xc.h>
#include "i2c.h"
#include "i2c_lcd.h"

// ------------------------------------
// Carita feliz personalizada
// ------------------------------------
unsigned char smile[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

void main()
{
    // --- Variables para la animación ---
    const char mensaje[] = "profe ponganos 5.0!";
    char linea1[17];

    int frame = 0;
    int i;
    int desp;
    int msg_len = sizeof(mensaje) - 1;

    // Inicialización de periféricos
    I2C_init();
    lcd_init();

    // Crear carácter personalizado
    lcd_create_char(0, smile);

    while (1)
    {
        // --------------------------------
        // Animación del texto
        // --------------------------------
        if (frame < msg_len)
        {
            for (i = 0; i < 16; i++)
            {
                if (i <= frame)
                {
                    linea1[i] = mensaje[i];
                }
                else
                {
                    linea1[i] = ' ';
                }
            }
        }
        else
        {
            desp = frame - msg_len + 1;

            for (i = 0; i < 16; i++)
            {
                if (i >= desp && (i - desp) < msg_len)
                {
                    linea1[i] = mensaje[i - desp];
                }
                else
                {
                    linea1[i] = ' ';
                }
            }
        }

        linea1[16] = '\0';

        // --------------------------------
        // Mostrar texto animado
        // --------------------------------
        lcd_set_cursor(0, 0);
        lcd_write_string(linea1);

        // --------------------------------
        // Mostrar carita en línea 2
        // --------------------------------
        lcd_set_cursor(1, 7);
        lcd_write_char(0);

        // --------------------------------
        // Control de animación
        // --------------------------------
        frame++;

        if (frame >= (msg_len + 16))
        {
            frame = 0;
        }

        __delay_ms(10);
    }
}