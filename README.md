# DBF
C++ implementations of Dijkstra's Algorithm and the Bellman-Ford Algorithm. Expected input for both algorithms is a graph file formatted in modified DIMACS. See any of the g files within this repo.

NOTE: if no target is desired, enter either 0 or a negative number, but if a target is specified the path from source to target will be printed

To compile Bellman Ford, use: make bellman-ford
To clean: make clean-bellman-ford
To run: ./bellman-ford graph-input source-vertex target-vertex


To compile Dijkstra's, use: make dijkstra
To clean: make clean-dijkstra
To run: ./dijkstra graph-input source-vertex target-vertex
