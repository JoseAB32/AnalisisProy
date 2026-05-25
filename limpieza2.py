import polars as pl


AIRPORT_COLUMNS = [
    "ID",
    "Nombre",
    "Ciudad",
    "Pais",
    "IATA",
    "ICAO",
    "Latitud",
    "Longitud",
    "Altitud",
    "ZonaHoraria",
    "DST",
    "TZ",
    "Tipo",
    "Fuente",
]

ROUTE_COLUMNS = [
    "Aerolinea",
    "ID_Aerolinea",
    "Origen_IATA",
    "ID_Origen",
    "Destino_IATA",
    "ID_Destino",
    "CodeShare",
    "Paradas",
    "Equipo",
]


def leer_datos() -> tuple[pl.DataFrame, pl.DataFrame]:
    airports = pl.read_csv(
        "airports.dat",
        separator=",",
        has_header=False,
        null_values=r"\N",
        new_columns=AIRPORT_COLUMNS,
    )

    routes = pl.read_csv(
        "routes.dat",
        separator=",",
        has_header=False,
        null_values=r"\N",
        new_columns=ROUTE_COLUMNS,
    )

    return airports, routes


def limpiar_airports(airports: pl.DataFrame) -> pl.DataFrame:
    return (
        airports
        .select([
            "ID",
            "Nombre",
            "Ciudad",
            "Pais",
            "IATA",
            "ICAO",
            "Latitud",
            "Longitud",
        ])
        .with_columns([
            pl.col("ID").cast(pl.Int64),
            pl.col("Latitud").cast(pl.Float64),
            pl.col("Longitud").cast(pl.Float64),
        ])
        .filter(
            pl.col("ID").is_not_null()
            & pl.col("Nombre").is_not_null()
            & pl.col("Latitud").is_not_null()
            & pl.col("Longitud").is_not_null()
        )
        .unique(subset=["ID"])
    )


def limpiar_routes(routes: pl.DataFrame, airports: pl.DataFrame) -> pl.DataFrame:
    airport_ids = airports.select("ID")

    routes_limpias = (
        routes
        .select([
            "Aerolinea",
            "ID_Aerolinea",
            "Origen_IATA",
            "ID_Origen",
            "Destino_IATA",
            "ID_Destino",
            "CodeShare",
            "Paradas",
            "Equipo",
        ])
        .filter(
            pl.col("ID_Origen").is_not_null()
            & pl.col("ID_Destino").is_not_null()
        )
        .with_columns([
            pl.col("ID_Origen").cast(pl.Int64),
            pl.col("ID_Destino").cast(pl.Int64),
            pl.col("Paradas").cast(pl.Int64),
        ])
        .join(
            airport_ids,
            left_on="ID_Origen",
            right_on="ID",
            how="semi",
        )
        .join(
            airport_ids,
            left_on="ID_Destino",
            right_on="ID",
            how="semi",
        )
        .unique()
    )

    return routes_limpias


def guardar_datos(df: pl.DataFrame, nombre_archivo: str) -> None:
    df.write_csv(nombre_archivo, separator="\t")


def mostrar_resumen(
    airports_original: pl.DataFrame,
    routes_original: pl.DataFrame,
    airports_limpios: pl.DataFrame,
    routes_limpias: pl.DataFrame,
) -> None:
    print("===== RESUMEN DE LIMPIEZA =====")
    print(f"Aeropuertos originales: {airports_original.height}")
    print(f"Aeropuertos limpios:    {airports_limpios.height}")
    print(f"Aeropuertos eliminados: {airports_original.height - airports_limpios.height}")
    print()
    print(f"Rutas originales:       {routes_original.height}")
    print(f"Rutas limpias:          {routes_limpias.height}")
    print(f"Rutas eliminadas:       {routes_original.height - routes_limpias.height}")



airports_original, routes_original = leer_datos()

airports_limpios = limpiar_airports(airports_original)
routes_limpias = limpiar_routes(routes_original, airports_limpios)

guardar_datos(airports_limpios, "aeropuertos_limpio.txt")
guardar_datos(routes_limpias, "rutas_limpio.txt")

mostrar_resumen(
    airports_original,
    routes_original,
    airports_limpios,
    routes_limpias,
)

