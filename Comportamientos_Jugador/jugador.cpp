#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <queue>
#include<vector>
using namespace std;


Action ComportamientoJugador::think(Sensores sensores){


	Action accion = actIDLE;
	int a,p,distancia,direccion;

	switch (last_action)
	{
	case actWALK:
		switch (current_state.brujula)
		{
			case norte: current_state.fil--; break;
			case noreste: current_state.fil--; current_state.col++; break;
			case este: current_state.col++; break;
			case sureste: current_state.fil++; current_state.col++; break;
			case sur: current_state.fil++; break;
			case suroeste: current_state.fil++; current_state.col--; break;
			case oeste: current_state.col--; break;
			case noroeste: current_state.fil--; current_state.col--; break;
		}
		frecuenciaMatriz[current_state.fil][current_state.col]++;

	break;
	case actRUN:
		switch (current_state.brujula)
		{
			case norte: 
				current_state.fil--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil--; 
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case noreste: 
				current_state.fil--;
				current_state.col++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil--; 
				current_state.col++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case este: 
				current_state.col++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.col++;
			break;
			case sureste: 
				current_state.fil++;
				current_state.col++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil++;
				current_state.col++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case sur: 
				current_state.fil++;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil++; 
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case suroeste: 
				current_state.fil++;
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil++;
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case oeste: 
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
			case noroeste: 
				current_state.fil--;
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
				current_state.fil--;
				current_state.col--;
				frecuenciaMatriz[current_state.fil][current_state.col]++;
			break;
		}
		break;
	case actTURN_SR:
		//actualizacion de girar 45 grados a la derecha
		a = current_state.brujula;
		a = (a+1)%8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;
	case actTURN_L:
		//actualizacion de girar 90 grados a la izquierda
		a = current_state.brujula;
		a = (a+6)%8;
		current_state.brujula = static_cast<Orientacion>(a);
		break;
	default:
		break;
	}

	//PARA METER LOS PRECIPICIOS
	if(PrimerTurno){
		metePrecipios(sensores);
		PrimerTurno = false;

		for(int i=0; i < 500 ; i++){
			for(int j=0; j < 500; j++){
				frecuenciaMatriz[i][j] = 0;
			}
		}
	}

	//Si te toca un lobo o te caes por un precipicio
	if(sensores.reset){
		bien_situado = false;
		tiene_bikini = false;
		tiene_zapatillas = false;
		celeste_encontrado = false;
		bikini_encontrado = false;
		zapatilla_encontrado = false;
		bateria_encontrado = false;
		current_state.brujula = norte;
		current_state.fil = 200;
		current_state.col = 200;
	}

	if(bien_situado)
	{
		Dibuja(sensores);
	} 

	// Descubre donde esta situado
	if((sensores.terreno[0]== 'G' && !bien_situado) || (sensores.nivel < 1 && !bien_situado)){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;

		for(int i=0; i < 500 ; i++){
			for(int j=0; j < 500; j++){
				frecuenciaMatriz[i][j] = 0;
			}
		}
	}



    // Si encuentra el bikini o las zapatilla para recordarlo
	if(sensores.terreno[0]== 'K' && !tiene_bikini){
		tiene_bikini = true;
	}
	if(sensores.terreno[0]== 'D' && !tiene_zapatillas){
		tiene_zapatillas = true;
	}

	if(sensores.terreno[0] == 'X' && sensores.bateria < 4300)
	{
		recargando = true;

	}else if(sensores.terreno[0] == 'X' && sensores.bateria >= 4500)
	{
		recargando = false;

	}
	
	// Si no ha encontrado celeste(bool para evitar que se meta todas las veces y solo en la primera o si es reseteado) , si no esta bien situado(igual, primera vez o reseteado)
	if(!celeste_encontrado && !bien_situado) 
	{
		int posicion =0;
		distancia = 0;;
		// Busca si hay un celeste(orientacion) en la vision
		for(int i=1; i < 16; i++){
			if(sensores.terreno[i] == 'G'){
				posicion = i;
				celeste_encontrado = true;
			}
		}
	
		if(celeste_encontrado)
		{
			ruta_celeste = CrearRuta(sensores,posicion);
		}
	}
	
	
	if(!tiene_bikini && !bikini_encontrado) 
	{
		p = -1;
		distancia = 0;
		// Busca si hay un bikini en la vision
		for(int i=1; i < 16; i++){
			if(sensores.terreno[i] == 'K'){
				p = i;
				bikini_encontrado = true;
			}
		}
	
		if(bikini_encontrado)
		{
			ruta_bikini = CrearRuta(sensores,p);
		}
	
	}

	if(!tiene_zapatillas && !zapatilla_encontrado) 
	{
		p = -1;
		distancia = 0;;
		// Busca si hay una zapatilla en la vision
		for(int i=1; i < 16; i++){
			if(sensores.terreno[i] == 'D'){
				p = i;
				zapatilla_encontrado = true;
			}
		}
			if(zapatilla_encontrado)
			{
				ruta_zapatilla = CrearRuta(sensores,p);
			}
	}


	// Si tiene la bateria por debajo de un umbral si encuentra se encuentra con alguna bateria va recordandola
	if((sensores.bateria < 3500) && !bateria_encontrado)
	{
		p = -1;
		distancia = 0;;
		// Busca si hay una bateria en la vision
		for(int i=1; i < sensores.terreno.size(); i++){
			if(sensores.terreno[i] == 'X'){
				p = i;
				bateria_encontrado = true;
			}
		}
	
		if(bateria_encontrado)
		{
			ruta_bateria = CrearRuta(sensores,p);
		}
	}




	// Elegimos la mejor opcion despues del analisis hecho.Si no hay algo que merezca la pena ir(rutas que son la prioridad) se elige segun la accion menos frecuente visitada
		if(!ruta_celeste.empty() && EsPosibleAccion(sensores,ruta_celeste.front()))
		{	
				accion = ruta_celeste.front();
				ruta_celeste.pop();

		}else if(!ruta_bateria.empty() && EsPosibleAccion(sensores,ruta_bateria.front()))
		{
				accion = ruta_bateria.front();
				ruta_bateria.pop();

		}else if(!ruta_zapatilla.empty() && EsPosibleAccion(sensores,ruta_zapatilla.front()))
		{
				accion = ruta_zapatilla.front();
				ruta_zapatilla.pop();

		}else if(!ruta_bikini.empty() && EsPosibleAccion(sensores,ruta_bikini.front()))
		{
				accion = ruta_bikini.front();
				ruta_bikini.pop();

		}else{
			accion = MejorOpcion(sensores);
		}
	





	last_action = accion;
	return accion;


}


	



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}


bool ComportamientoJugador::EsPosibleAccion(Sensores sensores, Action accion)
{
	if(accion == actTURN_SR || accion == actTURN_L)
	{
		return true;

	}else if(accion == actWALK)
	{
		if(Estalibre(sensores,2)){
			return true;
		}

	}else if (accion = actRUN){
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			return true;
		}
	}
	
	return false; // si ha llegado aqui es que no puede pasar ya haya lobo,aldeano o alguna casilla no posible
	
}


queue<Action> ComportamientoJugador::CrearRuta(Sensores sensores,int posicion){
	
	queue<Action> ruta;

	int distancia;
	//Cuan lejos tenemos que ir, lo he hecho asi para que se dividan mas los if y no ser tan dificil de ver
	if(posicion < 4){
		distancia = 1;
	}else if(posicion < 9){
		distancia = 2;
	}else if(posicion < 16){
		distancia = 3;
	}


	if(distancia == 1){
		if(posicion == 1){
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}else if(posicion == 2){
			ruta.push(actWALK);
		}else if(posicion == 3){
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}
	}else if(distancia == 2){
		if(posicion == 4){
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
			ruta.push(actWALK);
		}else if(posicion== 5){
			ruta.push(actWALK);
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}else if(posicion == 6){
			ruta.push(actRUN);
		}else if(posicion == 7){
			ruta.push(actWALK);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}else if(posicion == 8){
			ruta.push(actTURN_SR);
			ruta.push(actRUN);
		}
	}else if(distancia == 3){
		if(posicion == 9){
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actRUN);
			ruta.push(actWALK);
		}else if(posicion == 10){
			ruta.push(actWALK);
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actRUN);
		}else if(posicion == 11){
			ruta.push(actRUN);
			ruta.push(actTURN_L);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}else if(posicion == 12){
			ruta.push(actRUN);
			ruta.push(actWALK);
		}else if(posicion == 13){
			ruta.push(actRUN);
			ruta.push(actTURN_SR);
			ruta.push(actWALK);
		}else if(posicion == 14){
			ruta.push(actWALK);
			ruta.push(actTURN_SR);
			ruta.push(actRUN);
		}else if(posicion == 15){
			ruta.push(actTURN_SR);
			ruta.push(actRUN);
			ruta.push(actWALK);
		}
	
	}

	return ruta;
}

bool ComportamientoJugador::Estalibre(Sensores sensores,int posicion)
{
	
		if(sensores.agentes[posicion] == '_')
		{

		if(sensores.terreno[posicion] == 'S') // es posible si hay suelo pedrejoso
		{
			return true;

		}else if(sensores.terreno[posicion] == 'T') // es posible si hay suelo arenoso
		{
			return true;

		}else if(sensores.terreno[posicion] == 'A' && (tiene_bikini || bikini_encontrado)) // posible si tiene bikini pasar por el agua o esta encontrado y esta en la ruta
		{
			return true;

		}else if(sensores.terreno[posicion] == 'B' && (tiene_zapatillas || zapatilla_encontrado)) // posible si tiene zapatillas pasar por el bosque o esta encontrado y esta en la ruta
		{
			return true;

		}else if(sensores.terreno[posicion] == 'X') // es posible pasar por la bateria
		{
			return true;

		}else if(sensores.terreno[posicion] == 'G') // es posible pasar por la casilla celeste(posicionamiento)
		{
			return true;
		}else if(sensores.terreno[posicion] == 'K') // puede pasar por le bikini aunque no haga falta cogerlo
		{
			return true;

		}else if(sensores.terreno[posicion] == 'D') // puede pasar por las zapatillas aunque no haga falta cogerlas
		{
			return true;

		}
		}
	
	return false; // si ha llegado aqui es que no puede pasar ya haya lobo,aldeano o alguna casilla no posible
	
}


bool ComportamientoJugador::Estalibre2(Sensores sensores,int posicion)
{
	
		if(sensores.agentes[posicion] == '_')
		{

		if(sensores.terreno[posicion] == 'S') // es posible si hay suelo pedrejoso
		{
			return true;

		}else if(sensores.terreno[posicion] == 'T') // es posible si hay suelo arenoso
		{
			return true;

		}else if(sensores.terreno[posicion] == 'A' ) // posible si tiene bikini pasar por el agua o esta encontrado y esta en la ruta
		{
			return true;

		}else if(sensores.terreno[posicion] == 'B' ) // posible si tiene zapatillas pasar por el bosque o esta encontrado y esta en la ruta
		{
			return true;

		}else if(sensores.terreno[posicion] == 'X') // es posible pasar por la bateria
		{
			return true;

		}else if(sensores.terreno[posicion] == 'G') // es posible pasar por la casilla celeste(posicionamiento)
		{
			return true;
		}else if(sensores.terreno[posicion] == 'K') // puede pasar por le bikini aunque no haga falta cogerlo
		{
			return true;

		}else if(sensores.terreno[posicion] == 'D') // puede pasar por las zapatillas aunque no haga falta cogerlas
		{
			return true;

		}
		}
	
	return false; // si ha llegado aqui es que no puede pasar ya haya lobo,aldeano o alguna casilla no posible
	
}

// TESTEADO
void ComportamientoJugador::metePrecipios(Sensores sensores){

	int N = mapaResultado.size();

	for(int i=0; i < N; i++)
	{
		mapaResultado[0][i] = 'P';
		mapaResultado[i][0] = 'P';
		mapaResultado[N-1][i] = 'P';
		mapaResultado[i][N-1] = 'P';
	}

	for(int i=1; i < N; i++){
		mapaResultado[1][i] = 'P';
		mapaResultado[i][1] = 'P';
		mapaResultado[N-2][i] = 'P';
		mapaResultado[i][N-2] = 'P';
	}

	for(int i=1; i < N; i++){
		mapaResultado[2][i] = 'P';
		mapaResultado[i][2] = 'P';
		mapaResultado[N-3][i] = 'P';
		mapaResultado[i][N-3] = 'P';
	}

}

Action ComportamientoJugador::MejorOpcion(Sensores sensores){
	// ELIGE LA MEJOR OPCION DEPENDIENDO DE DONDE ESTE MIRANDO
	int mejorposicion,menor;
	Action accion = actIDLE;

	for(int i=1; i < sensores.terreno.size(); i++){
			if(sensores.agentes[i] == 'l'){
				accion = actTURN_SR;
			}
	}

	if(accion == actIDLE){
	if(current_state.brujula == 0 ) // NORTE
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
		/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil-2][current_state.col];
		}
		*/
		if(Estalibre(sensores,2) ){//&& frecuenciaMatriz[current_state.fil-1][current_state.col] < menor){
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col];

		}

		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil-1][current_state.col+1]  < menor){
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col+1];
		}

		if(Estalibre(sensores,1) && frecuenciaMatriz[current_state.fil-1][current_state.col-1] < menor ){
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col-1];
		}
	}

	if(current_state.brujula == 1 ) //  NORESTE
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
		/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil-2][current_state.col+2];
		}
		*/
		if(Estalibre(sensores,2)){ // && frecuenciaMatriz[current_state.fil-1][current_state.col+1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col+1];

		}
		
		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil][current_state.col+1]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil][current_state.col+1];
		}
		if(Estalibre(sensores,1) && frecuenciaMatriz[current_state.fil-1][current_state.col]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col];

		}

	}

	if(current_state.brujula == 2 ) // ESTE
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil][current_state.col+2];
		}
	*/
		if(Estalibre(sensores,2)){ //&& frecuenciaMatriz[current_state.fil][current_state.col+1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil][current_state.col+1];

		}
		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil+1][current_state.col+1]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col+1];
		}
		if(Estalibre(sensores,1) && frecuenciaMatriz[current_state.fil-1][current_state.col+1]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col+1];

		}
	}

	if(current_state.brujula == 3 ) // SURESTE
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil+2][current_state.col+2];
		}
	*/
		if(Estalibre(sensores,2)){// && frecuenciaMatriz[current_state.fil+1][current_state.col+1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col+1];

		}
		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil+1][current_state.col]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col];
		}
		if(Estalibre(sensores,1) &&  frecuenciaMatriz[current_state.fil][current_state.col+1]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil][current_state.col+1];

		}

	}

	if(current_state.brujula == 4 ) // SUR
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil+2][current_state.col];
		}
	*/
		if(Estalibre(sensores,2)){// && frecuenciaMatriz[current_state.fil+1][current_state.col] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col];

		}
		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil+1][current_state.col-1]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col-1];
		}
		if(Estalibre(sensores,1) &&  frecuenciaMatriz[current_state.fil+1][current_state.col+1]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col+1];
		}
	}

	if(current_state.brujula == 5 ) // SUROESTE
	{		
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil+2][current_state.col-2];
		}
	*/
		if(Estalibre(sensores,2)){// && frecuenciaMatriz[current_state.fil+1][current_state.col-1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col-1];

		}
		if(Estalibre(sensores,3) && frecuenciaMatriz[current_state.fil][current_state.col-1]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil][current_state.col-1];
		}
		if(Estalibre(sensores,1) &&  frecuenciaMatriz[current_state.fil+1][current_state.col]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col];

		}
	}

		if(current_state.brujula == 6 ) // OESTE
	{		
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil][current_state.col-2];
		}
	*/
		if(Estalibre(sensores,2)){// && frecuenciaMatriz[current_state.fil][current_state.col-1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil][current_state.col-1];

		}
		if(Estalibre(sensores,3) &&  frecuenciaMatriz[current_state.fil-1][current_state.col-1]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col-1];
		}
		if(Estalibre(sensores,1) &&  frecuenciaMatriz[current_state.fil+1][current_state.col-1]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil+1][current_state.col-1];

		}

	}


	if(current_state.brujula == 7 ) // NOROESTE
	{
		mejorposicion = -1;
		menor = frecuenciaMatriz[current_state.fil][current_state.col];
	/*
		if(Estalibre(sensores,2) && Estalibre(sensores,6)){
			mejorposicion = 6;
			menor = frecuenciaMatriz[current_state.fil-2][current_state.col-2];
		}
	*/
		if(Estalibre(sensores,2)){// && frecuenciaMatriz[current_state.fil-1][current_state.col-1] < menor){
			
			mejorposicion = 2;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col-1];

		}
		if(Estalibre(sensores,3) &&  frecuenciaMatriz[current_state.fil-1][current_state.col]  < menor){
			
			mejorposicion = 3;
			menor = frecuenciaMatriz[current_state.fil-1][current_state.col];
		}

		if(Estalibre(sensores,1) &&  frecuenciaMatriz[current_state.fil][current_state.col-1]  < menor){
			
			mejorposicion = 1;
			menor = frecuenciaMatriz[current_state.fil][current_state.col-1];

		}

	}
	}

	if(mejorposicion == -1 || mejorposicion == 3){
		accion = actTURN_SR;
	}
	if(mejorposicion == 2){
		accion = actWALK;
	}
	if(mejorposicion == 1){
		accion = actTURN_L;
	}
	if(mejorposicion == 6){
		accion = actRUN;
	}

	if(accion == actTURN_SR)
	{
		giros++;
	}else if(accion == actWALK){
		giros = 0;
	}

	if(giros > 4 ){
		if(Estalibre2(sensores,2)){
			accion = actWALK;
		}
	}

	return accion;
}

void ComportamientoJugador::Dibuja(Sensores sensores){
	
	if(sensores.nivel < 3){
		switch (current_state.brujula)
		{
			case norte: 
				mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
				mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[2];
				mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[6];
				mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[7];
				mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col-2] = sensores.terreno[10];
				mapaResultado[current_state.fil-3][current_state.col-1] = sensores.terreno[11];
				mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[12];
				mapaResultado[current_state.fil-3][current_state.col+1] = sensores.terreno[13];
				mapaResultado[current_state.fil-3][current_state.col+2] = sensores.terreno[14];
				mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[15]; 
			break;
			case noreste: 
				mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
				mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[6];
				mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col+1] = sensores.terreno[10];
				mapaResultado[current_state.fil-3][current_state.col+2] = sensores.terreno[11];
				mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[12];
				mapaResultado[current_state.fil-2][current_state.col+3] = sensores.terreno[13];
				mapaResultado[current_state.fil-1][current_state.col+3] = sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[15];
			break;
			case este: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[3];
			mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[5];
			mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[6];
			mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[8];
			mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil-2][current_state.col+3] = sensores.terreno[10];
			mapaResultado[current_state.fil-1][current_state.col+3] = sensores.terreno[11];
			mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[12];
			mapaResultado[current_state.fil+1][current_state.col+3] = sensores.terreno[13];
			mapaResultado[current_state.fil+2][current_state.col+3] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[15];
			break;
			case sureste:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[3];
			mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[5];
			mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[6];
			mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[8];
			mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil+1][current_state.col+3] = sensores.terreno[10];
			mapaResultado[current_state.fil+2][current_state.col+3] = sensores.terreno[11];
			mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[12];
			mapaResultado[current_state.fil+3][current_state.col+2] = sensores.terreno[13];
			mapaResultado[current_state.fil+3][current_state.col+1] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[15];

			break;
			case sur:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[5];
			mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[6];
			mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil+3][current_state.col+2] = sensores.terreno[10];
			mapaResultado[current_state.fil+3][current_state.col+1] = sensores.terreno[11];
			mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[12];
			mapaResultado[current_state.fil+3][current_state.col-1] = sensores.terreno[13];
			mapaResultado[current_state.fil+3][current_state.col-2] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[15];
			break;
			case suroeste: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[4];
			mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[5];
			mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[6];
			mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[7];
			mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[9];
			mapaResultado[current_state.fil+3][current_state.col-1] = sensores.terreno[10];
			mapaResultado[current_state.fil+3][current_state.col-2] = sensores.terreno[11];
			mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[12];
			mapaResultado[current_state.fil+2][current_state.col-3] = sensores.terreno[13];
			mapaResultado[current_state.fil+1][current_state.col-3] = sensores.terreno[14];
			mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[15];

			break;
			case oeste:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[1];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[4];
			mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[5];
			mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[6];
			mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[7];
			mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[9];
			mapaResultado[current_state.fil+2][current_state.col-3] = sensores.terreno[10];
			mapaResultado[current_state.fil+1][current_state.col-3] = sensores.terreno[11];
			mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[12];
			mapaResultado[current_state.fil-1][current_state.col-3] = sensores.terreno[13];
			mapaResultado[current_state.fil-2][current_state.col-3] = sensores.terreno[14];
			mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[15];
			break;
			case noroeste: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[1];
			mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[3];
			mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[4];
			mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[5];
			mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[6];
			mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[7];
			mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[8];
			mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[9];
			mapaResultado[current_state.fil-1][current_state.col-3] = sensores.terreno[10];
			mapaResultado[current_state.fil-2][current_state.col-3] = sensores.terreno[11];
			mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[12];
			mapaResultado[current_state.fil-3][current_state.col-2] = sensores.terreno[13];
			mapaResultado[current_state.fil-3][current_state.col-1] = sensores.terreno[14];
			mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[15];
			break;
		}
	}else{
		switch (current_state.brujula)
		{
			case norte: 
				mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
				mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[2];
				mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[7];
				mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col-2] = sensores.terreno[10];
				mapaResultado[current_state.fil-3][current_state.col+2] = sensores.terreno[14];
				mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[15]; 
			break;
			case noreste: 
				mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
				mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col+1] = sensores.terreno[5];
				mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col+1] = sensores.terreno[10];
				mapaResultado[current_state.fil-1][current_state.col+3] = sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[15];
			break;
			case este: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil-1][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[3];
			mapaResultado[current_state.fil-2][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil-1][current_state.col+2] = sensores.terreno[5];
			mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[8];
			mapaResultado[current_state.fil-3][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil-2][current_state.col+3] = sensores.terreno[10];
			mapaResultado[current_state.fil+2][current_state.col+3] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[15];
			break;
			case sureste:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[3];
			mapaResultado[current_state.fil][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil+1][current_state.col+2] = sensores.terreno[5];
			mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[8];
			mapaResultado[current_state.fil][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil+1][current_state.col+3] = sensores.terreno[10];
			mapaResultado[current_state.fil+3][current_state.col+1] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[15];

			break;
			case sur:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col+1] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[2];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col+2] = sensores.terreno[4];
			mapaResultado[current_state.fil+2][current_state.col+1] = sensores.terreno[5];
			mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[7];
			mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col+3] = sensores.terreno[9];
			mapaResultado[current_state.fil+3][current_state.col+2] = sensores.terreno[10];
			mapaResultado[current_state.fil+3][current_state.col-2] = sensores.terreno[14];
			mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[15];
			break;
			case suroeste: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col] = sensores.terreno[1];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col] = sensores.terreno[4];
			mapaResultado[current_state.fil+2][current_state.col-1] = sensores.terreno[5];
			mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[7];
			mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col] = sensores.terreno[9];
			mapaResultado[current_state.fil+3][current_state.col-1] = sensores.terreno[10];
			mapaResultado[current_state.fil+1][current_state.col-3] = sensores.terreno[14];
			mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[15];

			break;
			case oeste:
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil+1][current_state.col-1] = sensores.terreno[1];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[3];
			mapaResultado[current_state.fil+2][current_state.col-2] = sensores.terreno[4];
			mapaResultado[current_state.fil+1][current_state.col-2] = sensores.terreno[5];
			mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[7];
			mapaResultado[current_state.fil-2][current_state.col-2] = sensores.terreno[8];
			mapaResultado[current_state.fil+3][current_state.col-3] = sensores.terreno[9];
			mapaResultado[current_state.fil+2][current_state.col-3] = sensores.terreno[10];
			mapaResultado[current_state.fil-2][current_state.col-3] = sensores.terreno[14];
			mapaResultado[current_state.fil-3][current_state.col-3] = sensores.terreno[15];
			break;
			case noroeste: 
			mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			mapaResultado[current_state.fil][current_state.col-1] = sensores.terreno[1];
			mapaResultado[current_state.fil-1][current_state.col-1] = sensores.terreno[2];
			mapaResultado[current_state.fil-1][current_state.col] = sensores.terreno[3];
			mapaResultado[current_state.fil][current_state.col-2] = sensores.terreno[4];
			mapaResultado[current_state.fil-1][current_state.col-2] = sensores.terreno[5];
			mapaResultado[current_state.fil-2][current_state.col-1] = sensores.terreno[7];
			mapaResultado[current_state.fil-2][current_state.col] = sensores.terreno[8];
			mapaResultado[current_state.fil][current_state.col-3] = sensores.terreno[9];
			mapaResultado[current_state.fil-1][current_state.col-3] = sensores.terreno[10];
			mapaResultado[current_state.fil-3][current_state.col-1] = sensores.terreno[14];
			mapaResultado[current_state.fil-3][current_state.col] = sensores.terreno[15];
			break;
		}
	}

}


