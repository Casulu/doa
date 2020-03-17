gcc -g -std=c99 -Wall -o is_connected -I ../../datastructures-v1.0.8.2/include/ graph.c is_connected.c ../../datastructures-v1.0.8.2/src/dlist/dlist.c ../../datastructures-v1.0.8.2/src/array_1d/array_1d.c ../../datastructures-v1.0.8.2/src/table/table2.c
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/airmap1.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/airmap2.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/airmap3.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/5-big-map.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/6-single-edge.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/7-single-node.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/x-bad-map-format.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/4-standard-test.map
echo 
valgrind --leak-check=full --show-reachable=yes ./is_connected ../maps/3-directed-graph.map
echo 
