# Communication

A communication system which runs over shared memory. You have a sending process,
and a answering process. The sending process will lunch the answering process in the background.
You can send some sentences to the answering process, which is a Bot called *Robert*. He has
only a answer to some specific predefined questions/sentences. You can can have a look at them, and 
expand them if you wish, in the `AnsweringProcess.c` file.

# Execution
To compile use the __makefile__ or with the commandline:  
Sending process: `gcc -Werror -Wall SendingProcess.c util.c -o SendingProcess -lrt -lpthread -g`  
Answering Process: `gcc -Werror -Wall AnsweringProcess.c util.c -o AnsweringProcess -lrt -lpthread -g`

To execute run the sending process:  
`./SendingProcess`
