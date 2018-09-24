#include <stdlib.h>
#include "Graph.h"

void createGraph(struct Graph *graph, int nbMaxNodes, bool isDirected) {
    graph->isDirected = isDirected;
    graph->nbMaxNodes = nbMaxNodes;
    graph->adjList = (struct Neighbour **) malloc(graph->nbMaxNodes * sizeof(struct Neighbour *));
}

void addNode(struct Graph *graph, int node) {
    if (!(0 < node && node <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : addNode() -> node : %i, non comprise dans ]%i,%i]:\n", node, 0,
                graph->nbMaxNodes);
        return;
    }
    if (graph->adjList[node - 1] != NULL) {
        fprintf(stderr, "ERROR : addNode() -> node : %i, existe déjà: \n", node);
        return;
    }

    struct Neighbour *neighbour = (struct Neighbour *) malloc(sizeof(struct Neighbour));
    neighbour->neighbour = -1;
    neighbour->weigh = -1;
    neighbour->nextNeighbour = neighbour;
    neighbour->previousNeighbour = neighbour;
    graph->adjList[node - 1] = neighbour;
}

void addEdge(struct Graph *graph, int from, int weight, int to) {
    if (!(0 < from && from <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : addEdge() -> from : %i, non comprise dans ]%i,%i]:\n", from, 0,
                graph->nbMaxNodes);
        return;
    }
    if (!(0 < to && to <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : addEdge() -> to : %i, non comprise dans ]%i,%i]:\n", to, 0,
                graph->nbMaxNodes);
        return;
    }
    if (weight <= 0) {
        fprintf(stderr, "ERROR : addEdge() -> weight : %i, poids incorrect\n", weight);
        return;
    }
    if (graph->adjList[from - 1] == NULL) {
        fprintf(stderr, "ERROR : addEdge() -> from : %i, n'existe pas\n", from);
        return;
    }
    if (graph->adjList[to - 1] == NULL) {
        fprintf(stderr, "ERROR : addEdge() -> to : %i, n'existe pas\n", to);
        return;
    }

    struct Neighbour *parcours = graph->adjList[from - 1];
    while (parcours->neighbour != -1) {
        if (parcours->weigh == weight && parcours->neighbour == to) {
            fprintf(stderr, "ERROR : addEdge(), existe déja\n");
            return;
        }
        parcours = parcours->nextNeighbour;
    }

    struct Neighbour *neighbour = (struct Neighbour *) malloc(sizeof(struct Neighbour));
    neighbour->neighbour = to;
    neighbour->weigh = weight;
    neighbour->nextNeighbour = graph->adjList[from - 1];
    neighbour->previousNeighbour = graph->adjList[from - 1]->previousNeighbour;
    graph->adjList[from - 1]->previousNeighbour->nextNeighbour = neighbour;
    graph->adjList[from - 1]->previousNeighbour = neighbour;
    graph->adjList[from - 1] = neighbour;

    if (graph->isDirected == false && from != to) {
        struct Neighbour *neighbour2 = (struct Neighbour *) malloc(sizeof(struct Neighbour));
        neighbour2->neighbour = from;
        neighbour2->weigh = weight;
        neighbour2->nextNeighbour = graph->adjList[to - 1];
        neighbour2->previousNeighbour = graph->adjList[to - 1]->previousNeighbour;
        graph->adjList[to - 1]->previousNeighbour->nextNeighbour = neighbour2;
        graph->adjList[to - 1]->previousNeighbour = neighbour2;
        graph->adjList[to - 1] = neighbour2;
    }
}

void removeNode(struct Graph *graph, int node) {
    if (!(0 < node && node <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : removeNode() -> node : %i, non comprise dans ]%i,%i]:\n", node, 0,
                graph->nbMaxNodes);
        return;
    }

    if (graph->adjList[node - 1] == NULL) {
        fprintf(stderr, "ERROR : removeEdge() -> node : %i, n'existe pas\n", node);
        return;
    }

    struct Neighbour *current = graph->adjList[node - 1];
    struct Neighbour *next;

    while (current->neighbour != -1) {
        next = current->nextNeighbour;
        free(current);
        current = next;
    }

    free(current);
    graph->adjList[node - 1] = NULL;

    int passage;
    for (int i = 1; i <= graph->nbMaxNodes; i++) {

        if (graph->adjList[i - 1] == NULL) {
            continue;
        }

        passage = 0;
        current = graph->adjList[i - 1];

        while (current->neighbour != -1) {
            next = current->nextNeighbour;
            passage++;

            if (current->neighbour == node) {
                current->previousNeighbour->nextNeighbour = next;
                current->nextNeighbour->previousNeighbour = current->previousNeighbour;
                free(current);

                if (passage == 1) {
                    graph->adjList[i - 1] = next;
                    passage = 0;
                }
            }
            current = next;
        }
    }
}

void removeEdge(struct Graph *graph, int from, int weight, int to) {
    if (!(0 < from && from <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : removeEdge() -> from : %i, non comprise dans ]%i,%i]:\n", from, 0,
                graph->nbMaxNodes);
        return;
    }
    if (!(0 < to && to <= graph->nbMaxNodes)) {
        fprintf(stderr, "ERROR : removeEdge() -> to : %i, non comprise dans ]%i,%i]:\n", to, 0,
                graph->nbMaxNodes);
        return;
    }
    if (weight <= 0) {
        fprintf(stderr, "ERROR : removeEdge() -> weight : %i, poids incorrect\n", weight);
        return;
    }
    if (graph->adjList[from - 1] == NULL) {
        fprintf(stderr, "ERROR : removeEdge() -> from : %i, n'existe pas\n", from);
        return;
    }
    if (graph->adjList[to - 1] == NULL) {
        fprintf(stderr, "ERROR : removeEdge() -> to : %i, n'existe pas\n", to);
        return;
    }

    int passage = 0;
    struct Neighbour *parcours = graph->adjList[from - 1];
    while (parcours->neighbour != -1) {
        passage++;
        if (parcours->weigh == weight && parcours->neighbour == to) {
            parcours->previousNeighbour->nextNeighbour = parcours->nextNeighbour;
            parcours->nextNeighbour->previousNeighbour = parcours->previousNeighbour;
            if (passage == 1) {
                graph->adjList[from - 1] = parcours->nextNeighbour;
            }
            return;
        }
        parcours = parcours->nextNeighbour;
    }

    fprintf(stderr, "ERROR : removeEdge() -> (node,edge) : (%i,%i) , n'existe pas\n", to, weight);

}

void viewGraph(struct Graph *graph) {
    printf("# maximum number of nodes\n");
    printf("%i\n", graph->nbMaxNodes);
    printf("# directed\n");
    printf("%s\n", (graph->isDirected ? "y" : "n"));
    printf("# node: neighbours\n");
    for (int i = 0; i < graph->nbMaxNodes; ++i) {
        if (graph->adjList[i] == NULL) {
            continue;
        }
        printf("%i: ", i + 1);
        struct Neighbour *parcours = graph->adjList[i];
        while (parcours->neighbour != -1) {
            printf("(%i: %i), ", parcours->neighbour, parcours->weigh);
            parcours = parcours->nextNeighbour;
        }
        printf("\n");
    }
}

void loadGraph(struct Graph *graph, char *path) {
    FILE *in;
    in = fopen(path, "r");
    if (!in) {
        fprintf(stderr, "ERROR : loadGraph() -> fopen()\n");
        return;
    }

    int c;
    while ((c = getc(in)) != EOF)
        putchar(c);
    fclose(in);
}

void saveGraph(struct Graph *graph, char *path) {
    FILE *out;
    out = fopen(path, "w+");
    if (!out) {
        fprintf(stderr, "ERROR : saveGraph() -> fopen()\n");
        return;
    }

    fprintf(out, "# maximum number of nodes\n");
    fprintf(out, "%i\n", graph->nbMaxNodes);
    fprintf(out, "# directed\n");
    fprintf(out, "%s\n", (graph->isDirected ? "y" : "n"));
    fprintf(out, "# node: neighbours\n");
    for (int i = 0; i < graph->nbMaxNodes; ++i) {
        if (graph->adjList[i] == NULL) {
            continue;
        }
        fprintf(out, "%i: ", i + 1);
        struct Neighbour *parcours = graph->adjList[i];
        while (parcours->neighbour != -1) {
            fprintf(out, "(%i: %i), ", parcours->neighbour, parcours->weigh);
            parcours = parcours->nextNeighbour;
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

void quit(struct Graph *graph) {

}