# Detector de Arritmia y Fatiga Cardiaca

def leer_datos(nombre_archivo):

    pacientes = {}

    with open(nombre_archivo, "r") as archivo:

        for linea in archivo:

            datos = linea.strip().split(",")

            nombre = datos[0]

            frecuencias = list(map(int, datos[1:]))

            pacientes[nombre] = frecuencias

    return pacientes


def detectar_taquicardia(fc):

    contador = 0

    for i in range(len(fc)):

        if fc[i] > 100:

            contador += 1

            # Más de 5 lecturas consecutivas
            if contador > 5:

                minuto_inicio = i - 4
                minuto_fin = i + 1

                return True, minuto_inicio, minuto_fin

        else:
            contador = 0

    return False, 0, 0


def detectar_fatiga(fc):

    promedio_inicial = sum(fc[:10]) / 10
    promedio_final = sum(fc[-10:]) / 10

    incremento = ((promedio_final - promedio_inicial)
                  / promedio_inicial) * 100

    if incremento > 20:

        return True, promedio_inicial, promedio_final

    return False, promedio_inicial, promedio_final


# ---------------- PROGRAMA PRINCIPAL ---------------- #

pacientes = leer_datos("pacientes.txt")

while True:

    print("\n===== LISTA DE PACIENTES =====")

    for nombre in pacientes:
        print("-", nombre)

    nombre_paciente = input("\nIngrese paciente: ")

    if nombre_paciente in pacientes:

        fc = pacientes[nombre_paciente]

        print(f"\nEvaluando a {nombre_paciente}...")

        # TAQUICARDIA
        taqui, inicio, fin = detectar_taquicardia(fc)

        # FATIGA
        fatiga, prom1, prom2 = detectar_fatiga(fc)

        print("\n===== RESULTADOS =====")

        if taqui:

            print("Taquicardia Sostenida detectada")
            print(f"Desde el minuto {inicio} hasta el minuto {fin}")

        else:

            print("No presenta Taquicardia Sostenida")

        if fatiga:

            print("\nFatiga por Esfuerzo detectada")
            print(f"Promedio primeros 10 min: {prom1:.2f}")
            print(f"Promedio últimos 10 min: {prom2:.2f}")

        else:

            print("\nNo presenta Fatiga por Esfuerzo")

        if not taqui and not fatiga:

            print("\nPaciente sin complicaciones cardiacas")

    else:

        print("Paciente no encontrado")

    opcion = input("\n¿Desea evaluar otro paciente? (s/n): ")

    if opcion.lower() != "s":
        print("Programa finalizado")
        break