
header_file := task.h task_manager.h
cpp_file := main.cpp task.cpp  task_manager.cpp 

all: build

build: ${cpp_file} ${header_file}
	mkdir -p bin/
	g++ -o bin/task-cli ${cpp_file}

clean: 
	rm -rf bin/
