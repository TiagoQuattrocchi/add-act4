#include <stdio.h>
#include "pico/stdlib.h"
/*
 *  @brief  Inicializa los pines del ultrasonico
 *
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 */
void ultrasonico_init(uint8_t trigger, uint8_t echo) {
    /* Habilito el pin de Trigger */
    gpio_init(trigger);  
    /* Configuro el Trigger como salida */
    gpio_set_dir(trigger, true);
    /* Pongo un 0 en el pin de Trigger */
    gpio_put(trigger, false);
    /* Habilito el pin de Echo */
    gpio_init(echo);
    /* Configuro el Echo como entrada */
    gpio_set_dir(echo, false);
    
}

/*
 *  @brief  Obtiene la distancia en cm
 *  
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 * 
 *  @return distancia en cm
 */
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo) {
    /* Escribo un 1 en el Trigger */
    gpio_put(trigger, true);
    /* Espero 10 us con el pulso encendido */
    sleep_us(10);
    /* Escribo un 0 en el Trigger */
    gpio_put(trigger, false);
    /* Espero a que el pulso llegue al Echo */
    while(!gpio_get(echo));
    /* Mido el tiempo de cuando llega el pulso */
    absolute_time_t from = get_absolute_time();
    /* Espero a que el pulso del Echo baje a cero */
    while(gpio_get(echo));
    /* Mido el tiempo cuando termina el pulso */
    absolute_time_t to = get_absolute_time();
    /* Saco la diferencia de tiempo (el ancho del pulso) en us */
    int64_t pulse_width = absolute_time_diff_us(from, to);
    /* Calculo la distancia y la devuelvo */
    return pulse_width / 59.0;
}
/*
 * @brief Programa principal
 */
int main(void) {
    // Inicializacion del USB
    stdio_init_all();
    // Elegir un GPIO para trigger, echo y buzzer
    int trigger=0;
    int echo=1;
    int buzzer=2;
    // Inicializacion del ultrasonico
    ultrasonico_init(trig, echo);
    // Inicializo buzzer
    gpio_init(buzzer);
    gpio_set_dir(buzzer, true);

    while(1) {
        // Hago una lectura de distancia
        float d = ultrasonico_get_distance_cm(trig, echo);
        // Muestro por consola
        if(d<10){
            gpio_put(buzzer,true);
        }
        else{
            gpio_put(buzzer,false);
        }
        // Verifico distancia
            printf("distancia: %.2f\n ", d);
        // Espero
        sleep_ms(10);
    }
}
