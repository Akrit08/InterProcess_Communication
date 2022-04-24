# interProcess_Communication

The project is about getting the basic knowledge about process creation, 
and their management and learning the inter-process communication. 
In this project, I have used fork(), with which I can create new processes 
by duplicating the existing process from which it is called or simply, create child process. 
And mainly I have used a very important mechanism of inter-process communication in UNIX called pipe. 
Pipe is a buffer created via a pipe(fd) system call where fd[2] is an integer array that holds the 
file descriptors pointing to the two ends of the pipe. Within the array, one file descriptor fd[0] 
is for reading and another fd[1] is for writing. 
And when a process creates a pipe, and later calls a fork() system call, then the parent and child 
processes both hold a copy of the file descriptors associated with the pipe through which I can 
easily communicate between parent and child. The process is properly executed in the project where 
I will be able to find the median using the inter-process communication.
