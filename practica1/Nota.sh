#!/bin/bash

# Nombre del archivo para guardar la salida
output_file="ultimo_valor.txt"

# Limpiar el archivo de salida
> "$output_file"

suma=0
contador=0
# Función para obtener la última línea de la salida y agregarla al archivo
capturar_ultima_linea() {
    ultima_linea=$(./practica1SG ./mapas/mapa30_eval.map 3 "$1" "$2" "$3" 0 | tail -n 1)
    echo "$ultima_linea" >> "$output_file"
    echo "Ultima línea: $ultima_linea"
    local numero=$(echo "$ultima_linea" | awk '{print $NF}')
     if [[ "$numero" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        echo "Numero extraído por awk: $numero"
        suma=$(echo "$suma + $numero" | bc -l)
        ((contador++))
        echo "$suma"
        echo "Valor de nivel, fila y columna: $1, $3, $2"
    else
        echo "La última línea no es un número."
    fi
}

capturar_ultima_linea2() {
    ultima_linea=$(./practica1SG ./mapas/mapa50_eval.map 3 "$1" "$2" "$3" 0 | tail -n 1)
    echo "$ultima_linea" >> "$output_file"
    echo "Ultima línea: $ultima_linea"
    local numero=$(echo "$ultima_linea" | awk '{print $NF}')
     if [[ "$numero" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        echo "Numero extraído por awk: $numero"
        suma=$(echo "$suma + $numero" | bc -l)
        ((contador++))
        echo "$suma"
        echo "Valor de nivel, fila y columna: $1, $3, $2"
    else
        echo "La última línea no es un número."
    fi
}

capturar_ultima_linea3() {
    ultima_linea=$(./practica1SG ./mapas/mapa75_eval.map 3 "$1" "$2" "$3" 0 | tail -n 1)
    echo "$ultima_linea" >> "$output_file"
    echo "Ultima línea: $ultima_linea"
    local numero=$(echo "$ultima_linea" | awk '{print $NF}')
     if [[ "$numero" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        echo "Numero extraído por awk: $numero"
        suma=$(echo "$suma + $numero" | bc -l)
        ((contador++))
        echo "$suma"
        echo "Valor de nivel, fila y columna: $1, $3, $2"
    else
        echo "La última línea no es un número."
    fi
}


media30(){
    media=$(echo "$suma / $contador" | bc -l)
    echo "Media de 30 nivel $1: $media"
    suma=0
    contador=0
}

media50(){
    media=$(echo "$suma / $contador" | bc -l)
    echo "Media de 50 nivel $1: $media"
    suma=0
    contador=0
}

media75(){
    media=$(echo "$suma / $contador" | bc -l)
    echo "Media de 75 nivel $1: $media"
    suma=0
    contador=0
}


    echo "Mapa 75" >> "$output_file"
for((a = 0; a < 4; a++)); do
    echo "Mapa 75 nivel: $a" >> "$output_file"
    for (( j=4; j < 73; j++ )); do
        for((i=4; i < 73; i++)); do
             capturar_ultima_linea3 "$a" "$i" "$j"
        echo "Valor de nivel, fila y columna: $a, $j, $i" 
        done
    done
    media75 "$a" >> "$output_file"
done

echo "Fin del programa"
