all: xqagen tch reader
	@echo 'Now run: LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:../ ./xqagen < test.dtd > test.xqa; ./tch > test.xqm; ./reader '

xqagen: xqagen.cpp
	g++ -Wall -I.. -L.. xqagen.cpp -o xqagen -lxqueeze
tch: tch.cpp
	g++ -Wall -I.. -L.. tch.cpp -o tch -lxqueeze
reader: reader.cpp
	g++ -Wall -I.. -L.. reader.cpp -o reader -lxqueeze

clean:
	-rm xqagen tch reader test.xqa test.xqm *~
