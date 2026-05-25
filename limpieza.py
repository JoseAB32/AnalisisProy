import polars as pl


def leer_datos():
    df_ariports = pl.read_csv(
        "airports.dat", 
        separator=",", 
        has_header=False, 
        null_values=r"\N",
        new_columns=["ID", "Nombre", "Ciudad", "Pais", "IATA", "ICAO", "Latitud", "Longitud", "Otr1", "Otr2", "Otr3", "Otr4", "Otr5", "Otr6"] 
    )
    
    df_routes = pl.read_csv(
        "routes.dat", 
        separator=",", 
        has_header=False, 
        null_values=r"\N",
        quote_char=None,
        new_columns=["Aerolinea", "ID_Areolinea", "Origen_IATA", "ID_Origen", "Destino_IATA", "ID_Destino", "CodeShare", "Paradas", "Equipo"] 
    )

    
    return df_ariports, df_routes


def limpieza_datos(df_airports, df_routes):
    df_routes_limpio =df_routes.join(
        df_airports,
        left_on="ID_Origen",     
        right_on="ID",   
        how="semi"               
    ) 
    
    df_routes_limpio = df_routes_limpio.join(
        df_airports,
        left_on="ID_Destino",
        right_on="ID",
        how="semi"
    )

    return df_routes_limpio


def guardar_datos_en_txt(df, nombre_archivo):
    df.write_csv(nombre_archivo, separator="\t")


airports, routes = leer_datos()

routes = limpieza_datos(airports, routes)

guardar_datos_en_txt(routes, "rutas_limpio.txt")
guardar_datos_en_txt(airports, "aeropuertos_limpio.txt")

print(routes)