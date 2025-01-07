#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <queue>
#include<vector>
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;

};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      current_state.fil = current_state.col = 100;
      current_state.brujula = norte;
      last_action = actIDLE;
      girar_derecha = false;
      celeste_encontrado =false ; 
      tiene_bikini = false;
      bikini_encontrado = false;
      tiene_zapatillas = false;
      zapatilla_encontrado = false;
      bateria_encontrado= false;
      recargando = false;
      PrimerTurno = true;
      bien_situado = false;
      giros = 0;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
   

  private:
  
  // Declarar aquí las variables de estado
  state current_state;
  Action last_action;
  int girar_derecha;
  bool bien_situado;
  bool celeste_encontrado;
  bool tiene_bikini;
  bool bikini_encontrado;
  bool tiene_zapatillas;
  bool zapatilla_encontrado;
  bool bateria_encontrado;
  bool recargando;
  bool PrimerTurno;
  int giros;
  queue<Action> ruta_celeste;
  queue<Action> ruta_bateria;
  queue<Action> ruta_bikini;
  queue<Action> ruta_zapatilla;
  queue<Action> ruta_normal;
  int mejorposicion;
  int frecuenciaMatriz[500][500];
	
  
    //METODOS PRIVADOS
    bool EsPosibleAccion(Sensores sensores, Action accion);
    queue<Action> CrearRuta(Sensores sensores,int posicion );
    bool Estalibre(Sensores sensores,int posicion);
    bool Estalibre2(Sensores sensores,int posicion);
    void Dibuja(Sensores sensores);
    void metePrecipios(Sensores sensores);
    Action MejorOpcion(Sensores sensores);



};


#endif

