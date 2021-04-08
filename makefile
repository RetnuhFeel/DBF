dijkstra:
	g++ -O3 -g -Werror -Wall -o dijkstra Dijkstra.cpp;

clean-dijkstra:
	rm dijkstra;

bellman-ford:
	g++ -O3 -g -Werror -Wall -o bellman-ford BellmanFord.cpp;

clean-bellman-ford:
	rm bellman-ford;