[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/rb0M7Pn8)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=23804509&assignment_repo_type=AssignmentRepo)
# Lab07: Visualización en LCD 16x2 usando módulo I²C con microcontrolador PIC

## Integrantes

* [JUAN CAMILO SAMPER](https://github.com/CamiloSp22)

* [CRISTIAN FABIAN LOZANO](https://github.com/cristianfalozanoav)

## Documentación

En este laboratorio se trabajó la comunicación entre un microcontrolador PIC18F45K22 y una pantalla LCD 16x2 usando el protocolo I²C. La idea principal fue aprender a configurar el módulo MSSP del PIC en modo maestro para poder enviar información hacia un módulo PCF8574, el cual permite controlar la LCD utilizando únicamente dos líneas de comunicación.

Normalmente, una pantalla LCD conectada en modo paralelo necesita entre 6 y 8 pines del microcontrolador para funcionar correctamente. Esto puede ser un problema cuando se necesitan más entradas y salidas para conectar sensores, botones u otros dispositivos. Gracias al protocolo I²C y al módulo PCF8574, fue posible reducir todas esas conexiones a solo dos líneas: SDA y SCL, haciendo el circuito mucho más organizado y fácil de manejar.

Durante el desarrollo del laboratorio también se implementaron varias funciones en la LCD, como mostrar texto fijo, desplazar mensajes en pantalla y crear caracteres personalizados. Estas actividades ayudaron a entender mejor cómo se envían datos y comandos a través del bus I²C y cómo el microcontrolador interactúa con dispositivos externos.

Además, la práctica permitió reforzar temas importantes relacionados con comunicación serial, configuración de registros internos del PIC y manejo de periféricos digitales, mostrando una aplicación muy común en sistemas embebidos y proyectos electrónicos.

**Protocolo I²C**

El protocolo I²C es un sistema de comunicación serial muy utilizado en electrónica y microcontroladores porque permite conectar varios dispositivos usando solamente dos líneas de comunicación. Esto ayuda a simplificar bastante el diseño de los circuitos y reduce la cantidad de cables necesarios.

Las dos líneas utilizadas son:

* SDA, que se encarga de transportar los datos.
* SCL, que genera la señal de reloj para sincronizar toda la comunicación.

Una de las ventajas más importantes del I²C es que permite conectar varios dispositivos esclavos al mismo bus. Cada dispositivo tiene una dirección específica, lo que permite que el maestro pueda comunicarse con cada uno de ellos de manera independiente.

En este laboratorio, el PIC18F45K22 funcionó como maestro, mientras que el módulo PCF8574 actuó como esclavo encargado de controlar la pantalla LCD.

La comunicación se realiza enviando información en forma de bytes. Todo comienza con una condición Start generada por el maestro, luego se envía la dirección del dispositivo junto con el bit de lectura o escritura, después se transmiten los datos y finalmente se genera una condición Stop para terminar la comunicación.

**Comunicación half-duplex**

El protocolo I²C trabaja en modo half-duplex, lo que significa que la información solo puede viajar en una dirección a la vez. Es decir, mientras un dispositivo transmite datos, el otro únicamente recibe.

Aunque esto hace que la comunicación sea un poco más lenta comparada con otros protocolos, tiene la ventaja de reducir la cantidad de conexiones físicas y facilitar la integración de varios dispositivos en el mismo bus.

Esto es diferente al protocolo SPI, el cual trabaja en modo full-duplex y permite enviar y recibir información al mismo tiempo utilizando líneas separadas para transmisión y recepción.

**Módulo PCF8574**

El PCF8574 es un expansor de entradas y salidas digitales que permite controlar dispositivos paralelos mediante comunicación I²C.

En este laboratorio, el módulo recibió los datos seriales enviados por el PIC y los convirtió en señales paralelas que la pantalla LCD puede entender.

Gracias a este módulo fue posible controlar completamente la LCD utilizando solamente dos líneas del microcontrolador:

```RC3 → SCL```

```RC4 → SDA```

Esto representó una gran ventaja frente al modo paralelo tradicional, ya que redujo considerablemente la cantidad de cables y pines utilizados.

El PCF8574 normalmente utiliza la dirección base 0x27. Sin embargo, en la comunicación I²C la dirección debe incluir el bit de lectura o escritura, por lo que la dirección utilizada realmente fue:

```0x27 << 1 = 0x4E```

Por esa razón en el código se definió:

```#define LCD_ADDR 0x4E```

**Módulo MSSP del PIC18F45K22**

El PIC18F45K22 cuenta con el módulo MSSP (Master Synchronous Serial Port), diseñado para trabajar con protocolos seriales como SPI e I²C.

Cuando se configura en modo I²C maestro, el MSSP se encarga automáticamente de generar la señal de reloj, controlar la transmisión de datos y detectar diferentes eventos importantes durante la comunicación.

Entre las funciones principales del MSSP se encuentran:

* Generación de condiciones Start y Stop.
* Manejo del reloj serial.
* Envío y recepción de datos.
* Detección de colisiones.
* Manejo de bits ACK.
* Control de buffers de transmisión y recepción.

El funcionamiento correcto de este módulo depende de varios registros internos como SSPCON1, SSPCON2, SSPSTAT y SSPBUF, los cuales deben configurarse adecuadamente para garantizar una comunicación estable y sin errores.

## Diagramas

## Evidencias de implementación

VIDEO DE IMPLEMENTACION.

* [Practica Lab 7](https://youtu.be/l_CXYy5-IUk)

## Preguntas

1. ¿Por qué I²C se clasifica como half-duplex mientras que SPI es full-duplex? ¿Qué implicación práctica tiene esa diferencia para el control de una LCD?.

* El protocolo I²C se considera una comunicación half-duplex porque los datos solo pueden viajar en una dirección a la vez. Esto significa que mientras un dispositivo transmite información, el otro únicamente recibe, pero no pueden transmitir simultáneamente. En el bus I²C todos los dispositivos comparten la misma línea de datos SDA, por lo que la comunicación debe organizarse para evitar colisiones.

En cambio, SPI se clasifica como full-duplex porque puede enviar y recibir datos al mismo tiempo. Esto es posible gracias a que utiliza líneas separadas para transmisión y recepción de datos. SPI normalmente trabaja con las líneas MOSI (Master Out Slave In) y MISO (Master In Slave Out), permitiendo una comunicación simultánea en ambas direcciones.

2. En I2C_init() se asigna SSPCON1 = 0x28. Desglose ese valor bit a bit e identifique qué modo de operación del MSSP se está seleccionando y por qué se elige ese valor.

* Los cuatro bits menos significativos (SSPM3:SSPM0 = 1000) configuran el módulo MSSP en modo:

```I²C Master mode, clock = FOSC / (4 * (SSPADD + 1))```

Esto significa que el PIC actuará como maestro dentro del bus I²C y generará automáticamente la señal de reloj SCL.

El bit ```SSPEN = 1``` habilita físicamente el módulo MSSP y permite que los pines RC3 y RC4 funcionen como líneas SCL y SDA.

Este valor se utiliza porque permite que el microcontrolador controle completamente la comunicación I²C y genere la frecuencia adecuada mediante el registro SSPADD.

3. Las funciones I2C_start(), I2C_stop() e I2C_write() comparten el mismo patrón: activar un bit de control y luego esperar con while(!PIR1bits.SSPIF). ¿Qué representa la bandera SSPIF y por qué se limpia después de cada operación?.

* La bandera SSPIF pertenece al registro PIR1 y significa:

```Synchronous Serial Port Interrupt Flag```

Esta bandera indica que una operación del módulo MSSP ha finalizado correctamente.

Cuando se ejecuta una acción como:

* generar un Start,
* enviar un byte,
* recibir datos,
* generar un Stop,

el hardware del módulo MSSP trabaja automáticamente. Una vez termina esa tarea, el bit SSPIF cambia a 1 para avisar que la operación ya finalizó.
Esto se hace porque si no se limpia, el programa podría interpretar erróneamente que una nueva operación ya terminó, generando errores de sincronización en la comunicación I²C.

4. El fuse PBADEN = OFF está presente en la configuración. ¿Qué efecto tendría dejarlo en ON sobre los pines del puerto B, y por qué podría causar problemas si se usan esos pines como salidas digitales?.

* El fuse ```PBADEN``` controla el comportamiento inicial de algunos pines del puerto B en el PIC18F45K22, los pines RB0, RB1, RB2, RB3 y RB4 se configuran inicialmente como entradas analógicas después del reset.

5. Compare el control de la LCD en modo paralelo (lab04) con el modo I²C de este laboratorio. Mencione ventajas y desventajas de cada enfoque en términos de: cantidad de pines usados, velocidad de actualización y complejidad del código.

* El modo paralelo y el modo I²C permiten controlar la misma pantalla LCD, pero funcionan de manera muy diferente.

En el modo paralelo, la LCD se conecta directamente al microcontrolador mediante varias líneas de datos y control. Dependiendo de la configuración, pueden utilizarse entre 6 y 8 pines.

La principal ventaja del modo paralelo es su velocidad. Como los datos se transmiten directamente por varias líneas al mismo tiempo, la actualización de la pantalla es mucho más rápida.

Sin embargo, tiene la desventaja de consumir muchos pines del microcontrolador, lo que puede limitar la conexión de otros dispositivos.

Por otro lado, el modo I²C utiliza únicamente dos líneas de comunicación gracias al módulo PCF8574. Esto simplifica mucho el montaje físico y permite ahorrar una gran cantidad de pines.

La desventaja principal es que la comunicación se vuelve más lenta, ya que los datos deben transmitirse serialmente y además existe una sobrecarga adicional relacionada con el protocolo I²C.

En cuanto al código, el modo paralelo suele ser más simple porque el microcontrolador controla directamente las señales de la LCD. En cambio, el modo I²C requiere funciones adicionales para manejar el protocolo serial, las direcciones esclavas y la sincronización del bus.

6. El bus I²C permite conectar múltiples esclavos con solo dos hilos. Si se quisiera agregar un segundo módulo PCF8574 al mismo bus (por ejemplo, para controlar un segundo LCD), ¿qué cambio mínimo sería necesario en el hardware y en el código?

* Para agregar un segundo módulo PCF8574 al mismo bus I²C, el cambio más importante sería modificar la dirección del nuevo dispositivo.

En hardware, esto se realiza configurando de manera diferente los pines de dirección del PCF8574.

En el código, el cambio mínimo sería definir una segunda dirección:

``` #define LCD1_ADDR 0x4E``` 

``` #define LCD2_ADDR 0x4C``` 

Luego, al momento de enviar datos, se seleccionaría la dirección correspondiente según la pantalla que se quiera controlar.

## Conclusiones

Durante el desarrollo de este laboratorio fue posible comprender el funcionamiento del protocolo I²C y su aplicación en el control de una pantalla LCD 16x2 mediante el módulo expansor PCF8574.

Se comprobó que el uso de comunicación I²C representa una solución bastante eficiente para reducir la cantidad de pines utilizados en el microcontrolador, simplificando el diseño físico del circuito y permitiendo conectar múltiples dispositivos utilizando únicamente dos líneas.

También se logró entender el funcionamiento interno del módulo MSSP del PIC18F45K22 y la importancia de registros y banderas como SSPCON1 y SSPIF dentro del proceso de comunicación serial.

Además, la comparación entre el modo paralelo y el modo I²C permitió identificar las ventajas y limitaciones de cada enfoque, especialmente en términos de velocidad, simplicidad y optimización de recursos.

Finalmente, el laboratorio fortaleció conocimientos relacionados con programación de microcontroladores, protocolos seriales y manejo de periféricos electrónicos, los cuales son fundamentales en el desarrollo de sistemas embebidos.

## Referencias

[1] Microchip Technology Inc. (s.f.). PIC18F45K22 datasheet. Microchip Technology. Microchip PIC18F45K22

[2] Microchip Technology Inc. (s.f.). MPLAB X IDE. Microchip Technology. MPLAB X IDE

[3] Microchip Technology Inc. (s.f.). XC8 compiler user guide. Microchip Technology. XC8 Compiler

[4] Microchip Technology Inc. (s.f.). MSSP I²C master mode. Microchip Developer Help. Microchip Developer Help I2C

[5] NXP Semiconductors. (s.f.). PCF8574 remote 8-bit I/O expander for I²C-bus datasheet. NXP. NXP PCF8574

[6] NXP Semiconductors. (2007). UM10204 I²C-bus specification and user manual (Rev. 03). NXP. I2C Bus Specification

[7] SparkFun Electronics. (s.f.). I²C communication protocol. SparkFun Learn. SparkFun I2C Tutorial

[8] Last Minute Engineers. (s.f.). I2C LCD tutorial. Last Minute Engineers. LastMinuteEngineers LCD I2C Tutorial