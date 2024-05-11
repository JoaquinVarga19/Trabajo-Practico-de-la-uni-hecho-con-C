/*Integrantes del grupo:
Samuel Almaraz
Joaquin Cual
Joaquin Varga
Angel Siciliano
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define Max_Intentos 10
#define Tamanio_Numero 5
#define Max_Jugadores 10

struct Jugador
{
    char nombre_jugador[20];
    int intentos;
    double tiempo_minimo;
};

// Funcion para ingresar nombre
void Cargar_Nombre(struct Jugador *nombre)
{
    printf("Ingrese su nombre: ");
    scanf("%s", nombre->nombre_jugador);
}

// Funcion para generar el numero aleatorio
int Generar_Num_Aleatorio()
{
    return rand() % (99999 - 10000 + 1) + 10000;
}

// Funcion para separar en cifras
void Separar_en_Cifras(int Numero, int Cifras[Tamanio_Numero])
{
    int i;
    for (i = Tamanio_Numero - 1; i >= 0; i--)
    {
        Cifras[i] = Numero % 10;
        Numero = Numero / 10;
    }
}

// Funcion para validar el numero ingresado
bool Validar_Numero(int numero)
{
    if (numero < 10000 || numero > 99999)
    {
        printf("\nEl numero no tiene 5 cifras. Por favor, ingrese otro numero.\n");
        return false; // El numero no es valido
    }
    return true; // El numero es valido
}

// Funcion para jugar y comparar
void Jugar(struct Jugador *jugador)
{
    clock_t inicio = clock();

    int Numero_Aleatorio, Cifras_Aleatorio[Tamanio_Numero];
    Numero_Aleatorio = Generar_Num_Aleatorio();
    Separar_en_Cifras(Numero_Aleatorio, Cifras_Aleatorio);

    int Numero_Ingresado, Cifras_Ingresadas[Tamanio_Numero];
    int intentos = 0, i, j;

   // printf("Numero Aleatorio: %d\n", Numero_Aleatorio); //NOTA PARA EL PROFE: por si te mareas buscando los numeros solamente sacale el operador "//" en esta linea de codigo al principio de la linea obvio
    printf("\nBienvenido, %s al juego del numero del dia.\n", jugador->nombre_jugador);
    printf("Tienes que adivinar un numero de 5 cifras.\n");

    while (intentos < Max_Intentos)
    {
        printf("\nIntento %d/%d: Ingrese respuesta (5 cifras): ", intentos + 1, Max_Intentos);
        scanf("%d", &Numero_Ingresado);

        if (!Validar_Numero(Numero_Ingresado))
        {
            continue; // Vuelve a solicitar otro numero si no es valido
        }

        Separar_en_Cifras(Numero_Ingresado, Cifras_Ingresadas);
        intentos++;

        int correctos_posicion = 0;
        int correctos = 0;

        for (i = 0; i < Tamanio_Numero; i++)
        {
            if (Cifras_Ingresadas[i] == Cifras_Aleatorio[i])
            {
                printf("\nEl numero %d es correcto y esta en la posicion correcta.\n", Cifras_Ingresadas[i]);
                correctos_posicion++;
            }
        }

        for (i = 0; i < Tamanio_Numero; i++)
        {
            int encontrado = 0;
            if (Cifras_Ingresadas[i] != Cifras_Aleatorio[i])
            {
                for (j = 0; j < Tamanio_Numero; j++)
                {
                    if (Cifras_Ingresadas[i] == Cifras_Aleatorio[j] && !encontrado)
                    {
                        printf("\nEl numero %d es correcto pero esta en la posicion incorrecta.\n", Cifras_Ingresadas[i]);
                        encontrado = 1;
                        correctos++;
                    }
                }
            }
        }

        if (correctos_posicion == Tamanio_Numero)
        {
            printf("\n¡Felicidades! Encontraste el numero en %d intentos.\n", intentos);
            break;
        }
        else
        {
            printf("\nCifras correctas y en la posicion correcta: %d\n", correctos_posicion);
            printf("Cifras correctas pero en la posicion incorrecta: %d\n", correctos);
        }
    }

    if (intentos == Max_Intentos)
    {
        printf("\nLo siento, has agotado tus %d intentos. El numero correcto era %d.\n", Max_Intentos, Numero_Aleatorio);
    }

    clock_t fin = clock(); // Fin del juego
    double tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;

    // Asigna el tiempo_transcurrido y los intentos a jugador->tiempo_minimo y jugador->intentos
    jugador->tiempo_minimo = tiempo_transcurrido;
    jugador->intentos = intentos;
}

// Funcion para determinar si el jugador quiere jugar de nuevo
bool Jugar_De_Nuevo()
{
    char respuesta;
    printf("\n¿Quieres jugar de nuevo? (s/n): ");
    scanf(" %c", &respuesta);
    return (respuesta == 's' || respuesta == 'S');
}

// Funcion para mostrar el ranking de jugadores
void MostrarRanking(struct Jugador ranking[Max_Jugadores], int num_jugadores)
{
    printf("\nRanking de jugadores:\n");

    for (int i = 0; i < num_jugadores; i++)
    {
        printf("%d. %s - Intentos: %d, Tiempo: %.2lf segundos\n", i + 1, ranking[i].nombre_jugador, ranking[i].intentos, ranking[i].tiempo_minimo);
    }
}

// Funcion para actualizar el ranking con un nuevo jugador
void ActualizarRanking(struct Jugador nuevo_jugador, struct Jugador ranking[Max_Jugadores], int *num_jugadores)
{
    // Si el ranking no esta lleno o el nuevo jugador tiene un mejor rendimiento que el ultimo en el ranking
    if (*num_jugadores < Max_Jugadores || (nuevo_jugador.intentos < ranking[Max_Jugadores - 1].intentos) || (nuevo_jugador.intentos == ranking[Max_Jugadores - 1].intentos && nuevo_jugador.tiempo_minimo < ranking[Max_Jugadores - 1].tiempo_minimo))
    {
        // Agrega al nuevo jugador al ranking
        ranking[*num_jugadores] = nuevo_jugador;
        (*num_jugadores)++; // Incrementa el numero de jugadores en el ranking

        // Ordena el ranking en funcion de los intentos y el tiempo
        for (int i = 0; i < *num_jugadores - 1; i++)
        {
            for (int j = i + 1; j < *num_jugadores; j++)
            {
                if (ranking[i].intentos > ranking[j].intentos || (ranking[i].intentos == ranking[j].intentos && ranking[i].tiempo_minimo > ranking[j].tiempo_minimo))
                {
                    struct Jugador temp = ranking[i];
                    ranking[i] = ranking[j];
                    ranking[j] = temp;
                }
            }
        }

        // Si el ranking tiene mas de 10 jugadores, elimina al ultimo
        if (*num_jugadores > Max_Jugadores)
        {
            (*num_jugadores)--;
        }
    }
}

int main()
{
    srand(time(NULL));
    int num_jugadores = 0;
    struct Jugador ranking[Max_Jugadores];

    do
    {
        struct Jugador jugador_actual; // Crear una nueva instancia para cada jugador
        Cargar_Nombre(&jugador_actual);
        Jugar(&jugador_actual);
        ActualizarRanking(jugador_actual, ranking, &num_jugadores);
        MostrarRanking(ranking, num_jugadores);
    } while (Jugar_De_Nuevo());

    printf("\nGracias por jugar. Hasta luego, %s\n", ranking[0].nombre_jugador);

    return 0;
}