# DBF
C++ implementations of Dijkstra's Algorithm and the Bellman-Ford Algorithm. Expected input for both algorithms is a graph file formatted in modified DIMACS. See any of the g files within this repo.

NOTE: if no target is desired, enter either 0 or a negative number, but if a target is specified the path from source to target will be printed

To compile Bellman Ford, use: make bellman-ford
To clean: make clean-bellman-ford
To run: ./bellman-ford graph-input source-vertex target-vertex
NOTE: input file g1 is the same as g3, but g1 does not work with Bellman-Ford since it does not contain symmetric edges whereas g3 does and thus the output from Dijkstra's for g1 should match the output from Bellman-Ford for g3

To compile Dijkstra's, use: make dijkstra
To clean: make clean-dijkstra
To run: ./dijkstra graph-input source-vertex target-vertex
NOTE: g2 will not work on Dijkstra's as it contains negative edge weights
