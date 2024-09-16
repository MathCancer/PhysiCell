import networkx as nx
from networkx.drawing.nx_pydot import graphviz_layout
from networkx.drawing.nx_agraph import to_agraph, graphviz_layout
import matplotlib as mpl
import matplotlib.pyplot as plt
from IPython.display import Image
import numpy as np

def load_trajs(path, outputs):

    trajs = []
    all_states = set()

    with open(path + "/res_traj.txt", "r") as res_traj:
        id_trajectory = 0
        states = []

        for line in res_traj.readlines():
                
                if line.startswith("Trajectory"):
                    if id_trajectory > 0:
                        trajs.append(states)
                        states = []
                        
                    id_trajectory = int(line.split("#")[1])
                    current_time = 0

                elif line.startswith(" istate"):
                    state = line.split("\t")[1].strip()
                    state_array = " -- ".join(sorted([node for node in state.split(" -- ") if node in outputs]))
                    if state_array == '':
                        state_array = "<nil>"

                    all_states.add(state_array)
                    states.append(state_array)

                elif line == "\n":
                    pass

                else:
                    tokens = line.split("\t")
                    state = tokens[1].strip()
                    
                    state_array = " -- ".join(sorted([node for node in state.split(" -- ") if node in outputs]))

                    if state_array == '':
                        state_array = "<nil>"
                        
                    if (len(states) == 0 or states[len(states)-1] != state_array):

                        all_states.add(state_array)                    
                        states.append(state_array)            
            
            
    return trajs, all_states

def draw_graph_from_pandas(data, prog="neato"):
    G = nx.from_pandas_adjacency(data,  create_using=nx.DiGraph())

    edge_colors = [edge['weight'] for _, edge in G.edges.items()]

    pos = graphviz_layout(G, prog=prog)
    fig = plt.figure(figsize=(12,6), dpi=100)

    nx.draw(G, pos, with_labels=True, edgelist=[], node_size=0)
    cmap = mpl.colors.ListedColormap(plt.cm.Blues(np.linspace(0.2, 1, 100)))

    edges = nx.draw_networkx_edges(
        G,
        pos,
        arrowstyle="->",
        arrowsize=10,
        edgelist=G.edges().keys(),
        edge_color=edge_colors,
        edge_cmap=cmap,
        width=2,
        connectionstyle='arc3, rad = 0.1'
    )

    pc = mpl.collections.PatchCollection(edges, cmap=cmap)
    pc.set_array(edge_colors)
    plt.colorbar(pc, ax=fig.axes[0])
    plt.show()

    
def visit(node, start_node, array, proba, paths_dict, probas, ids_state, nb_loops):

    array.append(node)

    nonzeros = {ids_state[ind]:probas.loc[node,:].iloc[ind] for ind in probas.loc[node, :].to_numpy().nonzero()[0]}
    for state in sorted(nonzeros, key=nonzeros.get, reverse=True):

        if state == start_node:
            paths_dict[proba*nonzeros[state]] = array
        
        elif array.count(state) < nb_loops+1:
            visit(state, start_node, []+array, proba*nonzeros[state], paths_dict, probas, ids_state, nb_loops)
            
def compute_circuits(probas, ids_state, node, nb_loops):
    paths_dict = {}
    visit(node, node, [], 1, paths_dict, probas, ids_state, nb_loops)
    return paths_dict

def compute_stg_counts(trajs, all_states):
    state_ids = {state:ind for ind, state in enumerate(sorted(list(all_states))) }
    ids_state = {ind:state for ind, state in enumerate(sorted(list(all_states))) }
    stg_counts = np.zeros((len(all_states), len(all_states)))
    for i, t_traj in enumerate(trajs):
        for j, t_state in enumerate(t_traj):
            if j > 0:
                stg_counts[state_ids[t_traj[j-1]], state_ids[t_state]] += 1

    return stg_counts, state_ids, ids_state

def to_bits(state, nodes):
    if state == "<nil>":
        return tuple([0]*len(nodes))
    bits = []
    state_list = state.split(" -- ")
    for node in nodes:
        bits.append(1 if node in state_list else 0)
    return tuple(bits)
    
def to_istates(table, nodes):
    istates = {}
    for index, value in table.iloc[-1, :].items():
        istates.update({to_bits(index, nodes): value})
    return istates

def change_inputs(nodes, istates, name_value):
    new_istates = {}
    for bits, proba in istates.items():
        new_bits = list(bits)
        for name in name_value:
            new_bits[nodes.index(name)] = name_value[name]
        new_tuples = tuple(new_bits)
        if new_tuples not in new_istates.keys():
            new_istates[new_tuples] = proba
        else:
            new_istates[new_tuples] += proba   
    return new_istates
