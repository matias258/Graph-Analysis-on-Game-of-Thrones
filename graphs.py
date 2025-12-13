import pandas as pd
from collections import defaultdict, deque
import os 
import time # Importar la librería time para medir el rendimiento

# --- 1. Definiciones de Funciones ---

def build_graph(nodes_file, edges_file):
    """
    Lee los archivos de nodos y aristas, y construye un grafo no dirigido.
    Retorna: (graph, nodes_data) o (None, None) si falla la lectura.
    """
    if not os.path.exists(nodes_file) or not os.path.exists(edges_file):
        print(f"Error: No se encontró uno de los archivos ({nodes_file} o {edges_file}).")
        return None, None

    # Leer Nodos
    try:
        df_nodes = pd.read_csv(nodes_file, keep_default_na=False)
        nodes_data = {}
        for index, row in df_nodes.iterrows():
            node_id = row['Id']
            node_label = row['Label']
            # Almacenamos el ID y el Label
            nodes_data[node_id] = {'label': node_label}
    except Exception as e:
        print(f"Error al leer nodos: {e}")
        return None, None

    # Leer Aristas y construir el Grafo
    try:
        df_edges = pd.read_csv(edges_file, keep_default_na=False)
        graph = defaultdict(list)
        
        for index, row in df_edges.iterrows():
            source = row['Source']
            target = row['Target']
            
            weight = row['Weight'] if 'Weight' in df_edges.columns and pd.notna(row['Weight']) else 1
            
            # Grafo NO DIRIGIDO
            graph[source].append((target, int(weight)))
            graph[target].append((source, int(weight)))

    except Exception as e:
        print(f"Error al leer aristas: {e}")
        return None, None

    return graph, nodes_data


def bfs_shortest_path(graph, start_id, end_id):
    """
    Encuentra la distancia y el camino más corto entre dos nodos usando BFS.
    Retorna: (distancia, camino_de_IDs) o (-1, []) si es inalcanzable.
    """
    if start_id == end_id:
        return 0, [start_id]

    queue = deque([start_id])
    predecessor = {start_id: None}
    distance = {start_id: 0}

    found_path = False

    while queue:
        current_id = queue.popleft()

        if current_id == end_id:
            found_path = True
            break 
        
        if current_id in graph:
            for neighbor_id, weight in graph[current_id]:
                if neighbor_id not in distance:
                    distance[neighbor_id] = distance[current_id] + 1
                    predecessor[neighbor_id] = current_id 
                    queue.append(neighbor_id)
    
    # Reconstrucción del Camino
    if found_path:
        path_ids = []
        current = end_id
        
        while current is not None:
            path_ids.append(current)
            current = predecessor.get(current) 
        
        path_ids.reverse()
        return distance[end_id], path_ids
    
    return -1, []


# --- 2. Función Principal (main) ---

def main():
    # Registrar el tiempo de inicio
    start_time = time.time()
    
    # 2.1. Nombres de archivo
    NODES_FILE = 'got-nodes.csv'
    EDGES_FILE = 'got-edges.csv'

    # 2.2. Construcción del Grafo
    print("## Paso 1: Construcción del Grafo")
    graph, nodes = build_graph(NODES_FILE, EDGES_FILE)

    if graph is None:
        # La función build_graph ya imprime el error si falla
        return

    print(f"* Personajes cargados: **{len(nodes)}**")
    print(f"* Conexiones únicas: **{len(graph)}**\n")
    
    # 2.3. Entrada del Usuario
    print("## Paso 2: Búsqueda de Nodos")
    
    # Función auxiliar para obtener el ID de un personaje
    def get_id_from_input(prompt_text):
        while True:
            user_input = input(f"{prompt_text} (Ej: Jon, Arya, Jaime): ").strip()
            if not user_input:
                # Permite al usuario salir si deja la entrada vacía
                return None, None
            
            # Buscar el ID (que es igual al nombre en este dataset)
            if user_input in nodes:
                return user_input, nodes[user_input]['label']

            print(f"Personaje '{user_input}' no encontrado. Intente de nuevo con el nombre exacto.")
            
    # Obtener el nodo de inicio
    START_ID, start_label = get_id_from_input("Introduce el personaje de inicio")
    if START_ID is None: return
    
    # Obtener el nodo de destino
    END_ID, end_label = get_id_from_input("Introduce el personaje de destino")
    if END_ID is None: return

    # 2.4. Ejecución del Análisis BFS
    print("\n## Paso 3: Ejecutando BFS")
    print(f"Buscando ruta más corta entre **{start_label}** y **{end_label}**...")

    # Llamada a la función BFS
    distance, path_ids = bfs_shortest_path(graph, START_ID, END_ID)

    # 2.5. Imprimir Resultados
    print("\n--- RESULTADOS DEL ANÁLISIS ---")
    if distance != -1:
        print("=========================================")
        print(f"Camino encontrado.")
        print("=========================================")
        
        print(f"### Distancia Mínima (Saltos): **{distance}**")
        
        # Convertir IDs del camino a nombres (labels)
        path_labels = [nodes[node_id]['label'] for node_id in path_ids]
        
        print("\n### Camino Mínimo (Secuencia):")
        print("```")
        # Usamos un formato más limpio para el camino
        print(" " + " -> ".join(path_labels))
        print("```")
        

    else:
        print("=========================================")
        print(f"Resultado: No existe un camino entre {start_label} y {end_label} (Inalcanzable).")
        print("=========================================")
        
    # 2.6. Medición de Tiempo
    end_time = time.time()
    elapsed_time_ms = (end_time - start_time) * 1000 # Convertir a milisegundos

    print("\n--- MÉTRICA DE RENDIMIENTO ---")
    print(f"Tiempo total de ejecución (carga, grafo y búsqueda): **{elapsed_time_ms:.2f} ms**")
    print("-" * 40)
    
if __name__ == '__main__':
    main()