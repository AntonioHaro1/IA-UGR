#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <iostream>
#include <cmath>
#include <set>
#include <vector>
#include <stack>

struct stateN0{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator== (const stateN0 &x) const{
    if(jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c)
    {
      return true;
    }else 
    {
      return false;
    }
  }
};

struct nodoN0{
  //nodoN0* padre;
  stateN0 st;
  //bool TieneBikini;
  //bool TieneZapatillas;
  int valor;
  list<Action> secuencia;
  

  bool operator==(const nodoN0 &n) const{
    return (st == n.st);
  }

  bool operator<(const nodoN0 &n) const{
    if(st.jugador.f < n.st.jugador.f){
      return true;
    }else if(st.jugador.f == n.st.jugador.f and st.jugador.c < n.st.jugador.c)
    {
      return true;
    }else if(st.jugador.f == n.st.jugador.f and st.jugador.c == n.st.jugador.c and st.jugador.brujula < n.st.jugador.brujula){
      return true;
    }else
    {
      return false;
    }

  }
};



class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado

      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
  

    list<Action> AnchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    //list<Action> Anchuracolaborador(const stateN0 &inicio, const ubicacion &final,const ubicacion &colaborador, const vector<vector<unsigned char>> &mapa);
    bool FueraVision(const ubicacion &jugador, const ubicacion &colaborador);
    //list<Action> Buscacolaborador(const stateN0 &inicio,const ubicacion &colaborador, const vector<vector<unsigned char>> &mapa);
    list<Action> elegirAccion(const stateN0 &st,const vector<vector<unsigned char>> &mapa);
    bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
    ubicacion NextCasilla(const ubicacion &pos);
    stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa);
    //stateN0 applycolaborador(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa);
    bool Find(const stateN0 &item, const list<stateN0> &lista);
    bool Find(const stateN0 &item, const list<nodoN0> &lista);
    //bool Find2(const stateN0 &item,const vector<nodoN0> &n);
    void AnularMatriz(vector<vector<unsigned char>> &matriz);
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    //list<Action> Dijkstra(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    //list<Action> Aestrella(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    //int calcularValorFcolaborador(const ubicacion &nodo,const ubicacion &final);
    //int calcularValorFJugador(const ubicacion &nodo, const ubicacion &final);
    int valorCasillaJugador(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
    int valorCasillaJugadorConBikini(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
    int valorCasillaJugadorConZapatillas(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
    bool haPasadoPorBikini(nodoN0* nodo, const vector<vector<unsigned char>> &mapa);
    bool haPasadoPorZapatillas(nodoN0* nodo, const vector<vector<unsigned char>> &mapa);

  private:
    // Declarar Variables de Estado
    stateN0 c_state;  
    ubicacion goal;
    ubicacion colaborador;
    list<Action> plan; // Almacena el plan en ejecucion
    bool hayPlan ;     // Si de verdad indica que se esta siguienda un plan





};



#endif
