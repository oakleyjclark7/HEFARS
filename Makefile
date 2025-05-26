HEFARS: CustomVecLib.o Energy.o Event.o Propagation.o ChargeSharing.o Timeframe.o Pulseframe.o main.o
	g++ CustomVecLib.o Energy.o Event.o Propagation.o ChargeSharing.o Timeframe.o Pulseframe.o main.o -Ofast -o HEFARS 
	make clean

CustomVecLib.o: src\CustomVecLib.cpp
	g++ -c -std=c++17 -Ofast src\CustomVecLib.cpp -I.\include\

Energy.o: src\Energy.cpp
	g++ -c -std=c++17 -Ofast src\Energy.cpp -I.\include\

Event.o: src\Event.cpp
	g++ -c -std=c++17 -Ofast src\Event.cpp -I.\include\

Propagation.o: src\Propagation.cpp
	g++ -c -std=c++17 -Ofast src\Propagation.cpp -I.\include\

ChargeSharing.o: src\ChargeSharing.cpp
	g++ -c -std=c++17 -Ofast src\ChargeSharing.cpp -I.\include\

Timeframe.o: src\Timeframe.cpp
	g++ -c -std=c++17 -Ofast src\Timeframe.cpp -I.\include\

Pulseframe.o: src\Pulseframe.cpp
	g++ -c -std=c++17 -Ofast src\Pulseframe.cpp -I.\include\

main.o: src\main.cpp
	g++ -c -lstdc++fs -std=c++17 -Ofast src\main.cpp -I.\include\

clean:
	-del *.o 