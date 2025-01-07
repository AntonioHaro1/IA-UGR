#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <vector>
#include <stack>
/*
list<Action> AvanzaASaltosDeCaballo()
{
	list<Action> secuencia;
	secuencia.push_back(actSON_FORWARD);
	secuencia.push_back(actSON_FORWARD);
	secuencia.push_back(actSON_TURN_SR);
	secuencia.push_back(actSON_FORWARD);
	return secuencia;
}
*/


// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	if(sensores.nivel != 4)
	{
	if(!hayPlan){
		
		cout << " Calculando un nuevo plan \n";
		//cout << sensores.posF << sensores.posC;
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.colaborador.f = sensores.CLBposF;
		c_state.colaborador.c = sensores.CLBposC;
		c_state.colaborador.brujula = sensores.CLBsentido;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;
		colaborador.f = sensores.CLBposF;
		colaborador.c = sensores.CLBposC;
		colaborador.brujula = sensores.CLBsentido;
		switch (sensores.nivel)
		{
		case 0:
			cout << "nivel 0";
			plan = AnchuraSoloJugador_V3(c_state,goal,mapaResultado);
			break;
		case 1:
			cout << "nivel 1";
			//plan = Anchuracolaborador(c_state,goal,colaborador,mapaResultado);
			break;
		case 2:
			cout << "nivel 2";
			//plan = Dijkstra(c_state,goal,mapaResultado);
			break;
		case 3:
			cout << " nivel 3" << endl;
			//plan = Aestrella(c_state,goal,mapaResultado);
			break;
		}
		if(plan.size() > 0)
		{
			VisualizaPlan(c_state,plan);
			hayPlan = true;
		}

	}
	if(hayPlan and plan.size() > 0){
		cout << " Ejecutando la siguiente accion del plan \n";
		accion = plan.front();
		plan.pop_front();
	}
	if(plan.size() == 0)
	{
		cout << "Se completó el plan \n";
		hayPlan = false ; 
	}
	}else
	{
		cout << "nivel 4";
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

// NIVEL O	

list<Action> ComportamientoJugador::AnchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodoN0 current_node;
	list<nodoN0> frontier;
	set<nodoN0> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);



	frontier.push_back(current_node);


	while(!frontier.empty() and !SolutionFound)
	{

		frontier.pop_front();
		explored.insert(current_node);

		//Generar hijo actWALK
		nodoN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if(child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c)
		{
			current_node = child_walk;
			SolutionFound = true;

		}else if(explored.find(child_walk) == explored.end()){
			frontier.push_back(child_walk);
		}

		if(!SolutionFound){
			//Generar hijo actRUN
			nodoN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if(child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c)
			{
				current_node = child_run;
				SolutionFound = true;

			}else if(explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if(!SolutionFound)
		{
			//Generar hijo actTURN_L
			nodoN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}

			//Generar hijo actTURN_SR
			nodoN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_SR, current_node.st , mapa);
			child_turnr.secuencia.push_back(actTURN_SR);
			if(explored.find(child_turnr) == explored.end()){
				frontier.push_back(child_turnr);
			}
		}

		if(!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end())
			{
				frontier.pop_front();
				if(!frontier.empty())
				{
					current_node = frontier.front();
				}
			}
		}
	}

	if(SolutionFound)
	{
		cout << " solucion encontrada";
		plan = current_node.secuencia;
		//PintaPlan(current_node.secuencia);
	}


	return plan;

}
// AQUI EMPPIEZA TODO LO RELACIONADO AL NIVEL 1
/*
list<Action> ComportamientoJugador::Anchuracolaborador(const stateN0 &inicio, const ubicacion &final,const ubicacion &colaborador, const vector<vector<unsigned char>> &mapa)
{

	// Empezamos con el algoritmo de anchura basico para buscar el camino del jugador hasta el colaborador, la condicion de solucion es tener al colaborador en la vista
	
	nodoN0 current_node;
	list<nodoN0> frontier;
	set<nodoN0> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (FueraVision(current_node.st.jugador,colaborador));


	frontier.push_back(current_node);
	explored.insert(current_node);

	while(!frontier.empty() and !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		if(!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end())
			{
				frontier.pop_front();
				if(!frontier.empty())
				{
					current_node = frontier.front();
				}
			}
		}

		//Generar nodo forward
		nodoN0 child_walk = current_node;
		child_walk.st = apply(actFORWARD, current_node.st, mapa);
		if(FueraVision(current_node.st.jugador,colaborador))
		{
			child_walk.secuencia.push_back(actFORWARD);
			current_node = child_walk;
			SolutionFound = true;

		}else if(explored.find(child_walk) == explored.end()){
			child_walk.secuencia.push_back(actFORWARD);
			frontier.push_back(child_walk);
		}



		if(!SolutionFound)
		{
			//Generar hijo actTURN_L
			nodoN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			if(explored.find(child_turnl) == explored.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}

			//Generar hijo actTURN_R
			nodoN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st , mapa);
			if(explored.find(child_turnr) == explored.end()){
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
		}
	}

	// existe el camino del jugador al colaborador
	if(SolutionFound)
	{
		cout << " solucion encontrada";
		current_node = frontier.front();
		plan = current_node.secuencia;
	}

	// ahora que el colaborador ya esta en la vision del jugado empieza a buscar el camino del colaborador hasta la meta
	nodoN0 current_node2;
	list<nodoN0> frontier2;
	list<nodoN0> explored2;
	list<Action> plan2; 
	current_node2.st = inicio;

	bool SolutionFound2 = (current_node2.st.colaborador.f == final.f and current_node2.st.colaborador.c == final.c);
	
	frontier2.push_back(current_node2);


	while(!frontier2.empty() and !SolutionFound2)
	{
		frontier2.pop_front();
		explored2.push_back(current_node2);

		nodoN0 child_walk = current_node2;
		child_walk.st = applycolaborador(actSON_FORWARD, current_node2.st, mapa);
		if(child_walk.st.colaborador.f == final.f and child_walk.st.colaborador.c == final.c)
		{
			child_walk.secuencia.push_back(actSON_FORWARD);
			current_node2 = child_walk;
			SolutionFound2 = true;

		}else if(!Find(child_walk.st,frontier2) and !Find(child_walk.st,explored2)){
			child_walk.secuencia.push_back(actSON_FORWARD);
			frontier2.push_back(child_walk);
		}

		if(!SolutionFound2)
		{
			//Generar hijo actTURN_SL
			nodoN0 child_turnl = current_node2;
			child_turnl.st = applycolaborador(actSON_TURN_SL,current_node2.st, mapa);
			if(!Find(child_turnl.st,frontier2) and !Find(child_turnl.st,explored2)){
				child_turnl.secuencia.push_back(actSON_TURN_SL);
				frontier2.push_back(child_turnl);
			}

			//Generar hijo actTURN_SR
			nodoN0 child_turnr = current_node2;
			child_turnr.st = applycolaborador(actSON_TURN_SR, current_node2.st , mapa);
			if(!Find(child_turnr.st,frontier2) and !Find(child_turnr.st,explored2)){
				child_turnr.secuencia.push_back(actSON_TURN_SR);
				frontier2.push_back(child_turnr);
			}
		}

		if(!SolutionFound2 and !frontier2.empty()){
			current_node2 = frontier2.front();
			while(!frontier2.empty() and Find(current_node2.st,explored2))
			{
				frontier2.pop_front();
				if(!frontier2.empty())
				{
					current_node2 = frontier2.front();
				}
			}
		}
	}


	// Se ha encontrado solucion del colaborador
	if(SolutionFound2)
	{
		plan2 = current_node2.secuencia;
	}

	// COMPROBAMOS QUE NINGUNA ACCION SE HAGA FUERA DE LA VISION DEL JUGADOR Y SI ESTA EN EL CAMINO
	list<Action> planfinal = plan;

	stateN0 seguimiento;
	seguimiento.jugador = current_node.st.jugador;
	seguimiento.colaborador = inicio.colaborador;

	for(auto it = plan2.begin(); it != plan2.end(); it++)
	{
		// tenemos el state seguimiento para saber si en algun momento saldra de su vista o chocaran
		if((*it) == actSON_FORWARD)
		{

			seguimiento.colaborador = NextCasilla(seguimiento.colaborador);

		}else if((*it) == actSON_TURN_SL)
		{

			seguimiento.colaborador.brujula = static_cast<Orientacion>((seguimiento.colaborador.brujula + 7) % 8);
		}else if((*it) == actSON_TURN_SR)
		{
			seguimiento.colaborador.brujula = static_cast<Orientacion>((seguimiento.colaborador.brujula + 1) % 8);
		}

		// para ver si en el siguiente movimiento chocará con el jugador
		if(seguimiento.jugador.f == seguimiento.colaborador.f and seguimiento.jugador.c == seguimiento.colaborador.c)
		{
			// Si van a choca elige la primera opcion libre de izquierda derecha arriba o abajo para que el jugador pueda moverse
			if(mapa[seguimiento.jugador.f+1][seguimiento.jugador.c] != 'P' and mapa[seguimiento.jugador.f+1][seguimiento.jugador.c] != 'M' and mapa[seguimiento.jugador.f+1][seguimiento.jugador.c] == mapa[seguimiento.colaborador.f][seguimiento.colaborador.c])
			{
				if(seguimiento.jugador.brujula == norte)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.f+1;
				}else if(seguimiento.jugador.brujula == sur)
				{
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.brujula = static_cast<Orientacion>((seguimiento.jugador.brujula + 4) % 8);
					seguimiento.jugador.f+1;
				}else if(seguimiento.jugador.brujula == oeste)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actTURN_R);
					seguimiento.jugador.brujula = static_cast<Orientacion>((seguimiento.jugador.brujula + 2) % 8);
					seguimiento.jugador.f+1;
				}else if(seguimiento.jugador.brujula == este)
				{
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.brujula = static_cast<Orientacion>((seguimiento.jugador.brujula + 6) % 8);
					seguimiento.jugador.f+1;
				}
			}else if(mapa[seguimiento.jugador.f-1][seguimiento.jugador.c] != 'P' and mapa[seguimiento.jugador.f-1][seguimiento.jugador.c] != 'M' and mapa[seguimiento.jugador.f-1][seguimiento.jugador.c] == mapa[seguimiento.colaborador.f][seguimiento.colaborador.c])
				{
				if(seguimiento.jugador.brujula == sur)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.f-1;
				}else if(seguimiento.jugador.brujula == norte)
				{
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.f-1;
				}else if(seguimiento.jugador.brujula == oeste)
				{
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.f-1;
				}else if(seguimiento.jugador.brujula == este)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actTURN_R);
					seguimiento.jugador.f-1;
				}	
			}else if(mapa[seguimiento.jugador.f][seguimiento.jugador.c+1] != 'P' and mapa[seguimiento.jugador.f][seguimiento.jugador.c+1] != 'M' and mapa[seguimiento.jugador.f][seguimiento.jugador.c+1] == mapa[seguimiento.colaborador.f][seguimiento.colaborador.c])
			{
				if(seguimiento.jugador.brujula == oeste)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c+1;
				}else if(seguimiento.jugador.brujula == este)
				{
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c+1;
				}else if(seguimiento.jugador.brujula == norte)
				{
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c+1;
				}else if(seguimiento.jugador.brujula == sur)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actTURN_R);
					seguimiento.jugador.c+1;
				}
			}else if(mapa[seguimiento.jugador.f][seguimiento.jugador.c-1] != 'P' and mapa[seguimiento.jugador.f][seguimiento.jugador.c-1] != 'M' and mapa[seguimiento.jugador.f][seguimiento.jugador.c-1] == mapa[seguimiento.colaborador.f][seguimiento.colaborador.c])
			{
				if(seguimiento.jugador.brujula == este)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c-1;
				}else if(seguimiento.jugador.brujula == oeste)
				{
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c-1;
				}else if(seguimiento.jugador.brujula == sur)
				{
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actTURN_L);
					seguimiento.jugador.c-1;
				}else if(seguimiento.jugador.brujula == norte)
				{
					planfinal.push_back(actTURN_L);
					planfinal.push_back(actFORWARD);
					planfinal.push_back(actTURN_R);
					planfinal.push_back(actTURN_R);
					seguimiento.jugador.c-1;
				}
			}
		// no funciona correctamente por eso esta comentado pero aqui deberia de si esta fuera de la vision del jugador buscara el camino menor
		// para tener al colaborador en vision
		}/*
		if(FueraVision(seguimiento.jugador,seguimiento.colaborador))
		{
			list<Action> plancolaborador;
			plancolaborador = Buscacolaborador(seguimiento,seguimiento.colaborador,mapa);
			planfinal.splice(planfinal.end(),plancolaborador);
		}
		
		planfinal.push_back(*it);
		
	}


	return planfinal;
	
	

}
*/
/*
// Esta es la funcion adicional que busca al colaborador pero no es usada porque no funcionaba
list<Action> ComportamientoJugador::Buscacolaborador(const stateN0 &inicio,const ubicacion &colaborador, const vector<vector<unsigned char>> &mapa)
{	
	
	nodoN0 current_node;
	list<nodoN0> frontier;
	set<nodoN0> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (!FueraVision(current_node.st.jugador,colaborador));


	frontier.push_back(current_node);
	explored.insert(current_node);

	while(!frontier.empty() and !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		if(!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end())
			{
				frontier.pop_front();
				if(!frontier.empty())
				{
					current_node = frontier.front();
				}
			}
		}

		nodoN0 child_walk = current_node;
		child_walk.st = apply(actFORWARD, current_node.st, mapa);
		if(!FueraVision(current_node.st.jugador,colaborador))
		{
			child_walk.secuencia.push_back(actFORWARD);
			current_node = child_walk;
			SolutionFound = true;

		}else if(explored.find(child_walk) == explored.end()){
			child_walk.secuencia.push_back(actFORWARD);
			frontier.push_back(child_walk);
		}



		if(!SolutionFound)
		{
			//Generar hijo actTURN_L
			nodoN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			if(explored.find(child_turnl) == explored.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}

			//Generar hijo actTURN_R
			nodoN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st , mapa);
			if(explored.find(child_turnr) == explored.end()){
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
		}

	}

	if(SolutionFound)
	{
		cout << " solucion encontrada";
		current_node = frontier.front();
		plan = current_node.secuencia;
	}

	return plan;

}

// Funcion para comprobar si la ubicaciones que pasa por parametros esta en la vision
bool ComportamientoJugador::FueraVision(const ubicacion &jugador, const ubicacion &colaborador)
{
	bool resultado = false;

	switch (jugador.brujula)
	{
	case norte:
		if(colaborador.f == jugador.f-3)
		{
			if(colaborador.c > jugador.c-4 and colaborador.c < jugador.c+4)
			{
				resultado = true;
			}
		}else if(colaborador.f == jugador.f-2)
		{
			if(colaborador.c > jugador.c-3 and colaborador.c < jugador.c+3)
			{
				resultado = true;
			}
		}else if(colaborador.f == jugador.f-1)
		{
			if(colaborador.c > jugador.c-2 and colaborador.c < jugador.c+2)
			{
				resultado = true;
			}
		}
		
		break;

	case este:
		if(colaborador.c == jugador.c+3)
		{
			if(colaborador.f > jugador.f-4 and colaborador.f < jugador.f+4)
			{
				resultado = true;
			}
		}else if(colaborador.c == jugador.c+2)
		{
			if(colaborador.f > jugador.f-3 and colaborador.f < jugador.f+3)
			{
				resultado = true;
			}
		}else if(colaborador.c == jugador.c+1)
		{
			if(colaborador.f > jugador.f-2 and colaborador.f < jugador.f+2)
			{
				resultado = true;
			}
		}
	break;
	case sur:
		if(colaborador.f == jugador.f+3)
		{
			if(colaborador.c < jugador.c+4 and colaborador.c > jugador.c-4)
			{
				resultado = true;
			}
		}else if(colaborador.f == jugador.f+2)
		{
			if(colaborador.c < jugador.c+3 and colaborador.c > jugador.c-3)
			{
				resultado = true;
			}
		}else if(colaborador.f == jugador.f+1)
		{
			if(colaborador.c < jugador.c+2 and colaborador.c > jugador.c-2)
			{
				resultado = true;
			}
		}
		
	break;
	case oeste:
		if(colaborador.c == jugador.c-3)
		{
			if(colaborador.f > jugador.f-4 and colaborador.f < jugador.f+4)
			{
				resultado = true;
			}
		}else if(colaborador.c == jugador.c-2)
		{
			if(colaborador.f > jugador.f-2 and colaborador.f < jugador.f+2)
			{
				resultado = true;
			}
		}else if(colaborador.c == jugador.c-1)
		{
			if(colaborador.f > jugador.f-1 and colaborador.f < jugador.f+1)
			{
				resultado = true;
			}
		}
	break;
	}

	return resultado;
}
/*
// apply de colaborador ya que se mueven diferente
stateN0 ComportamientoJugador::applycolaborador(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa)
 {
	stateN0 st_result = st;
	ubicacion sig_ubicacion;

	switch(a)
	{
		case actSON_FORWARD:
			sig_ubicacion = NextCasilla(st.colaborador);
			if(CasillaTransitable(sig_ubicacion,mapa))
			{
				st_result.colaborador = sig_ubicacion;
			}
		break;
		case actSON_TURN_SL:
			st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 7) % 8);
			break;

		case actSON_TURN_SR:
			st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1) % 8);
			break;
	}
	return st_result;
 }
*/
/*
// AQUI EMPIEZA TODO LO RELACIONADO AL NIVEL 2

list<Action> ComportamientoJugador::Dijkstra(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	
	nodoN0 current_node;
	set<nodoN0> visitados;
	list<Action> plan;
	vector<nodoN0> novisitados;
	current_node.st = inicio;
	current_node.valor = 0;
	/*
	Todo esto era para que tuviese en cuenta el bikini y las zapatillas
	current_node.padre = &current_node;
	current_node.TieneBikini = false;
	current_node.TieneZapatillas = false;
	current_node.padre->TieneBikini = false;
	current_node.padre->TieneZapatillas = false;

	
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);

	novisitados.push_back(current_node);



	while (!novisitados.empty() and !SolutionFound)
	{

		// buscamos el mejor candidato en la lista de nodos no visitados
		nodoN0 mejor = novisitados[0];
		int mejorvalor = novisitados[0].valor;
		int borrar = 0;

		for(int i = 1; i< novisitados.size(); i++){

			if(novisitados[i].valor < mejorvalor)
			{
				mejorvalor = novisitados[i].valor;
				mejor = novisitados[i];
				borrar = i;
			}
		}

		current_node = mejor; 
		current_node.valor = mejorvalor;


		if(current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c)
		{
			SolutionFound = true;
		}

		visitados.insert(current_node);
		novisitados.erase(novisitados.begin() + borrar);

		// añadimos los demas


		nodoN0 child_walk = current_node;
		child_walk.st = apply(actFORWARD, current_node.st, mapa);
		/*
		Esto era para que heredase el atributo dependiendo del camino es decir sus nodo padres 
		child_walk.padre = &current_node;
		child_walk.st = apply(actFORWARD, current_node.st, mapa);
		child_walk.TieneBikini  = child_walk.padre->TieneBikini;
		child_walk.TieneZapatillas =  child_walk.padre->TieneZapatillas;
		

		Despues de actualizar si tiene zapatillas o no comprube si esta situado en alguna casilla objeto
		para cambiar el objeto
		if(mapa[child_walk.st.jugador.f][child_walk.st.jugador.c] == 'D' )
		{
			if(child_walk.TieneBikini)
			{
				child_walk.TieneBikini = false;	
			}
			child_walk.TieneZapatillas = true;
		}else if(mapa[child_walk.st.jugador.f][child_walk.st.jugador.c] == 'K' )
		{
			if(child_walk.TieneZapatillas)
			{
				child_walk.TieneZapatillas = false;
			}
			child_walk.TieneBikini = true;
		}
		

		if(visitados.find(child_walk) == visitados.end()){
			child_walk.secuencia.push_back(actFORWARD);
			/*
			dependiendo del objeto usaba una funcion difente para calcular el valor actual que tenia el nodo
			if(child_walk.TieneBikini)
			{
				child_walk.valor = current_node.valor + valorCasillaJugadorConBikini(child_walk.st.jugador, mapa);
			}else if(child_walk.TieneZapatillas)
			{
				child_walk.valor = current_node.valor + valorCasillaJugadorConZapatillas(child_walk.st.jugador, mapa);
			}else
			{
				child_walk.valor = current_node.valor + valorCasillaJugador(child_walk.st.jugador,mapa);
			}
			
			child_walk.valor = current_node.valor + valorCasillaJugador(child_walk.st.jugador,mapa);
			novisitados.push_back(child_walk);
		}

		if(!SolutionFound)
		{
			//Generar hijo actTURN_L
			nodoN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			/*
			Actualiza los atributos por el camino que haya seguido
			child_turnl.padre = &current_node;
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			child_turnl.TieneBikini = child_turnl.padre->TieneBikini;
			child_turnl.TieneZapatillas = child_turnl.padre->TieneZapatillas;
			
			if(visitados.find(child_turnl) == visitados.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				novisitados.push_back(child_turnl);
			}
			

			//Generar hijo actTURN_R
			nodoN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st , mapa);
			/*
			Actualiza los atributos por el camino que haya seguido
			child_turnr.padre = &current_node;
			child_turnr.st = apply(actTURN_R, current_node.st , mapa);
			child_turnr.TieneBikini = child_turnl.padre->TieneBikini;
			child_turnr.TieneZapatillas = child_turnl.padre->TieneZapatillas;
			
			if(visitados.find(child_turnr) == visitados.end()){
				child_turnr.secuencia.push_back(actTURN_R);
				novisitados.push_back(child_turnr);
			}
		}

	}
	
	if(SolutionFound)
	{
		cout << " solucion encontrada";
		plan = current_node.secuencia;
	}
	
	return plan;

}
*/

int ComportamientoJugador::valorCasillaJugador(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
 {
	int valor = 1;

	if(mapa[x.f][x.c] == 'T')
	{
		valor =  2;

	}else if(mapa[x.f][x.c] == 'B')
	{
		valor = 50;

	}else if(mapa[x.f][x.c] == 'A')
	{
		valor = 100;
	}

	return valor;


 }
/*

	funciones que ayudaban con los items que el jugador tenia
 int ComportamientoJugador::valorCasillaJugadorConBikini(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
 {
	int valor = 1;

	if(mapa[x.f][x.c] == 'T')
	{
		valor =  2;

	}else if(mapa[x.f][x.c] == 'B')
	{
		valor = 50;

	}else if(mapa[x.f][x.c] == 'A')
	{
		valor = 10;
	}

	return valor;


 }

  int ComportamientoJugador::valorCasillaJugadorConZapatillas(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
 {
	int valor = 1;

	if(mapa[x.f][x.c] == 'T')
	{
		valor =  2;

	}else if(mapa[x.f][x.c] == 'B')
	{
		valor = 15;

	}else if(mapa[x.f][x.c] == 'A')
	{
		valor = 100;
	}

	return valor;


 }

 // este fue otro tipo de intento, ya que parecia que el heredar no funcionaba, que comprobara de uno en uno si su padre era una casilla objeto

 bool ComportamientoJugador::haPasadoPorBikini( nodoN0* nodo, const vector<vector<unsigned char>> &mapa)
 {
	bool resultado = false;
	while(nodo->padre->valor == -1)
	{
		if(mapa[nodo->st.jugador.f][nodo->st.jugador.c] == 'K')
		{
			resultado = true;
		}
		nodo = nodo->padre;
	}

	return resultado;

 }

  bool ComportamientoJugador::haPasadoPorZapatillas(nodoN0* nodo, const vector<vector<unsigned char>> &mapa)
 {
	bool resultado = false;
	while(nodo->padre != nullptr)
	{
		if(mapa[nodo->st.jugador.f][nodo->st.jugador.c] == 'D')
		{
			resultado = true;
		}
		nodo = nodo->padre;
	}

	return resultado;

 }
*/



// NIVEL 3

/*
intento del nivel 3, en el algoritmo se hace al A* para ir del jugador a la meta para comprobar que funcionaba y luego adaptarlo

list<Action> ComportamientoJugador::Aestrella(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	
	nodoN0 current_node;
	vector<nodoN0> abiertos;
	vector<nodoN0> cerrados;
	current_node.st = inicio;
	current_node.valor = 0;
	current_node.valorF = calcularValorFJugador(inicio.jugador,final);
	int coste;

	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);

	abiertos.push_back(current_node);


	while (!abiertos.empty() and !SolutionFound)
	{

		// buscamos el mejor candidato en la lista de nodos abiertos

		nodoN0 mejor = abiertos[0];
		int mejorvalor = abiertos[0].valor + abiertos[0].valorF;
		int borrar = 0;

		for(int i = 1; i< abiertos.size(); i++){
			if(abiertos[i].valor < mejorvalor)
			{
				mejorvalor =  (abiertos[i].valor + abiertos[i].valorF);
				mejor = abiertos[i];
			}
		}

		current_node = mejor; 
		current_node.valor = mejor.valor;
		current_node.valorF = mejor.valorF;
		
		if(current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c)
		{
			SolutionFound = true;
		}

		//cerrados.push_back(current_node);
		//abiertos.erase(cerrados.begin() + borrar);
			
		cerrados.push_back(current_node);
		
		for(int i = 0; i < abiertos.size(); i++)
			{
				if(abiertos[i].st == current_node.st and abiertos[i].valor == current_node.valor and abiertos[i].valorF == current_node.valorF)
				{
					abiertos.erase(abiertos.begin() + i);
				}
			}

		// añadimos al nodo forward
		nodoN0 child_walk = current_node;
		child_walk.st = apply(actFORWARD, current_node.st, mapa);
		//actualizamos el valor
		coste = child_walk.valor + valorCasillaJugador(child_walk.st.jugador,mapa);
		
		// si en abiertos hay un nodo con la misma posicion pero el valor,secuencia y valor puede ser diferente
		if(Find2(child_walk.st,abiertos))
		{
			//cout << "se mete primer find"<< endl;
			// for hasta encontrar el nodo con la misma posicion
			for(int i = 0; i < abiertos.size(); i++)
			{
				//cuando lo encontramos hacemos un if para ver si el nodo que esta en abierto tiene menor valor del que venimos
				if(abiertos[i].st.jugador.f == child_walk.st.jugador.f and abiertos[i].st.jugador.c == child_walk.st.jugador.c)
				{
					
					// si es menos entonces borramos el que teniamos en abiertos para meterlo despues
					if(child_walk.valor < abiertos[i].valor)
					{
						cout << "borra abiertos" << endl;
						abiertos.erase(abiertos.begin() + i);
						child_walk.secuencia.push_back(actFORWARD);
						child_walk.valor = coste;
						child_walk.valorF = calcularValorFJugador(child_walk.st.jugador,final);
						abiertos.push_back(child_walk); 
					}
				}
			}
		}
		// si en cerrados hay un nodo con la misma posicion
		if(Find2(child_walk.st,cerrados))
		{
			// for hasta encontrar el nodo con la misma posicion
			for(int i = 0; i < cerrados.size(); i++)
			{
				//cuando lo encontramos hacemos un if para ver si el nodo que esta en abierto tiene menor valor del que venimos
				if(cerrados[i].st.jugador.f == child_walk.st.jugador.f and cerrados[i].st.jugador.c == child_walk.st.jugador.c)
				{
					// si es menos entonces borramos el que teniamos en abiertos para meterlo despues
					if(child_walk.valor < cerrados[i].valor)
					{
						cout << "borra cerrados" << endl;
						//cout << "se mete segundo find2"<< endl;
						cerrados.erase(cerrados.begin() + i);
						child_walk.secuencia.push_back(actFORWARD);
						child_walk.valor = coste;
						child_walk.valorF = calcularValorFJugador(child_walk.st.jugador,final);
						cerrados.push_back(child_walk); 
					}
				}
			}
		}

		if(!Find2(child_walk.st,abiertos) )
		{

			child_walk.secuencia.push_back(actFORWARD);
			child_walk.valor = coste;
			child_walk.valorF = calcularValorFJugador(child_walk.st.jugador,final);
			abiertos.push_back(child_walk); 
		}

		//cout << "abiertos" <<abiertos.size() << endl;
		//cout <<  "cerrados" <<cerrados.size() << endl;




		if(!SolutionFound)
		{
			//Generar hijo actTURN_L
			nodoN0 child_turnl = current_node; // posible fallo?
			child_turnl.st = apply(actTURN_L,current_node.st, mapa);
			if(!Find2(child_turnl.st,abiertos)){
				child_turnl.secuencia.push_back(actTURN_L);
				abiertos.push_back(child_turnl);
			}

			//Generar hijo actTURN_R
			nodoN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st , mapa);
			if(!Find2(child_turnr.st,abiertos)){
				child_turnr.secuencia.push_back(actTURN_R);
				abiertos.push_back(child_turnr);
			}
		}
	
	}
	
	if(SolutionFound)
	{
		cout << " solucion encontrada";
		plan = current_node.secuencia;
	}
	
    list<Action> plan;
	plan.push_back(actIDLE);
	return plan;
	

}


// como el colaborador puede caminar en diagonal la heuristica del colaborador sera la distanciachebyshev 
int ComportamientoJugador::calcularValorFcolaborador(const ubicacion &nodo,const ubicacion &final)
{
	int distanciaX = abs(abs(nodo.f) - abs(final.f));
	int distanciaY = abs(abs(nodo.c) - abs(final.c));
	int distancia = max(distanciaX,distanciaY);

	return distancia;
}

// como el jugador solo puede girar 90 grados entonces la heuristica del jugador sera la manhattan
int ComportamientoJugador::calcularValorFJugador(const ubicacion &nodo, const ubicacion &final)
{
	int distancia = abs(abs(nodo.f) - abs(final.f)) + abs(abs(nodo.c) - abs(final.c));

	return (int)distancia;
}

*/
 // Devuelve una ubicacion en el mapa si es transitable

 bool ComportamientoJugador::CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
 {
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
 }

// Devuelve la ubicacion del avance segun el sentido
 ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos)
 {
	ubicacion next = pos;
	switch(pos.brujula)
	{
		case norte:
			next.f = pos.f -1;
			break;
		case noreste:
			next.f = pos.f -1;
			next.c = pos.c +1;;
			break;
		case este:
			next.c = pos.c +1;
			break;
		case sureste:
			next.f = pos.f +1;
			next.c = pos.c +1;
			break;
		case sur:
			next.f = pos.f +1;
			break;
		case suroeste:
			next.f = pos.f +1;
			next.c = pos.c -1;
			break;
		case oeste:
			next.c = pos.c -1;
			break;
		case noroeste:
			next.f = pos.f -1;
			next.c = pos.c -1;
			break;
		default:
			break;

	}

	return next;
 }

 stateN0 ComportamientoJugador::apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa)
 {
	stateN0 st_result = st;
	ubicacion sig_ubicacion,sig_ubicacion2;

	switch(a)
	{
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) and !(sig_ubicacion.f == st.colaborador.c and sig_ubicacion.c == st.colaborador.c))
			{
				sig_ubicacion2 =  NextCasilla(sig_ubicacion);
				if(CasillaTransitable(sig_ubicacion2,mapa) and !(sig_ubicacion2.f == st.colaborador.c and sig_ubicacion2.c == st.colaborador.c))
				{
					st_result.jugador = sig_ubicacion2;
				}
			}
		break;

		case actWALK:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) and !(sig_ubicacion.f == st.colaborador.c and sig_ubicacion.c == st.colaborador.c))
			{
				st_result.jugador = sig_ubicacion;
			}
		break;
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
			break;

		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1) % 8);
			break;
	}
	return st_result;
 }


/*
  bool ComportamientoJugador::Find(const stateN0 &item, const list<nodoN0> &lista)
 {
	bool resultado = false;
	for(auto it = lista.begin(); it != lista.end(); it++)
	{
		if(it->st.colaborador.f == item.colaborador.f and it->st.colaborador.c == item.colaborador.c and it->st.colaborador.brujula == item.colaborador.brujula)
		{
			resultado = true;
		}
	}
	return resultado;
 }
 /*
/*
   bool ComportamientoJugador::Find2(const stateN0 &item, const vector<nodoN0> &lista)
 {
	bool resultado = false;
	for(auto it = lista.begin(); it != lista.end(); it++)
	{
		if(it->st.jugador.f == item.jugador.f and it->st.jugador.c == item.jugador.c)
		{
			resultado = true;
		}
	}
	return resultado;
 }*/

  bool ComportamientoJugador::Find(const stateN0 &item, const list<nodoN0> &lista)
 {
	auto it = lista.begin();
	while(it != lista.end() and !(it->st == item))
	{
		it++;
	}

	return (!(it == lista.end()));

 }

 
 

void ComportamientoJugador::AnularMatriz(vector<vector<unsigned char>> &matriz)
{
	for(int i = 0; i< matriz.size(); i++)
	{
		for(int j = 0 ; j < matriz[0].size(); j++)
		{
			matriz[i][j] = 0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

void PintaPlan(const list<Action> &plan){
	auto it = plan.begin();
	while(it != plan.end()){
		if (*it == actWALK){
			cout << "W";
		}else if (*it == actRUN){
			cout << "R";
		}else if (*it == actTURN_SR){
			cout << "r";
		}else if (*it == actTURN_L){
			cout << "L";
		}else if (*it == act_CLB_WALK){
			cout << "cW";
		}else if (*it == act_CLB_TURN_SR){
			cout << "cr";
		}else if (*it == act_CLB_STOP){
			cout << "cS";
		}else if (*it == actIDLE){
			cout << "I";
		}else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

