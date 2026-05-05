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

 void main()
{
    // --- Variables para la animación ---
    const char mensaje[] = "Hola mundo!"; // Puedes cambiar el texto aquí
    char linea1[17];                      // Buffer de 16 caracteres + fin de cadena
    int frame = 0;                        // Cuadro actual
    int i;
    int desp;
    int msg_len = sizeof(mensaje) - 1;    // Calcula automáticamente el largo del texto

    // Inicialización de periféricos
    I2C_init();
    lcd_init();

    while (1)
    {
        // 1. Fase de aparición: letra por letra desde la izquierda
        if (frame < msg_len) {
            for (i = 0; i < 16; i++) {
                if (i <= frame) {
                    linea1[i] = mensaje[i];
                } else {
                    linea1[i] = ' '; // Rellena el resto con espacios en blanco
                }
            }
        } 
        // 2. Fase de desplazamiento: empujar todo el texto hacia la derecha
        else {
            desp = frame - msg_len + 1; // Cuántos espacios empujar el texto
            
            for (i = 0; i < 16; i++) {
                if (i >= desp && (i - desp) < msg_len) {
                    linea1[i] = mensaje[i - desp];
                } else {
                    linea1[i] = ' '; // Rellena los huecos con espacios
                }
            }
        }

        linea1[16] = '\0'; // Asegura que la cadena termine correctamente

        // 3. Imprimir el frame actual en la LCD
        lcd_set_cursor(0, 0);       // Coloca el cursor en la primera línea
        lcd_write_string(linea1);   // Escribe el buffer completo

        // 4. Lógica de avance y reinicio
        frame++;
        
        // El ciclo total es el largo del texto + los 16 espacios de la pantalla
        if (frame >= (msg_len + 16)) { 
            frame = 0; // Reinicia la animación
        }

        __delay_ms(200); // Pausa para controlar la velocidad (ajusta a tu gusto)
    }
}
    
