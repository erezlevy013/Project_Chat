#makefile for ServerManager

TARGET = ServerManager.out
TARGET2 = ClientManager.out
TARGET3 = Sender.out
TARGET4 = Listener.out
CC = gcc
CFLAGS = -ansi -g -pedantic -Wall -Werror
OBJS = ServerNet.o ServerManager.o User.o UserManager.o Group.o GroupManager.o Protocol.o
OBJS2 = ClientNet.o ClientManager.o Protocol.o UI.o Chat.o
OBJS3 = Sender.o
OBJS4 = Listener.o
#LIBPATH = ~/myPrograms/DataStructures/GenDSLibrary/

 $(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L. -lGenDSLib

Protocol.o: Protocol.c Protocol.h Limits.h GenVector.h
	$(CC) -I $(LIBPATH) $(CFLAGS) -c Protocol.c

User.o: User.c User.h Limits.h GenList.h ListItr.h ListFunc.h
	$(CC)  $(CFLAGS) -c User.c

UserManager.o: UserManager.c UserManager.h User.h Limits.h HashMap.h GenList.h
	$(CC)  $(CFLAGS) -c UserManager.c

Group.o: Group.c Group.h Limits.h HashMap.h
	$(CC)  $(CFLAGS) -c Group.c

GroupManager.o: GroupManager.c GroupManager.h Group.h Limits.h HashMap.h GenQueue.h GenVector.h
	$(CC) $(CFLAGS) -c GroupManager.c

ServerNet.o: ServerNet.c ServerNet.h Limits.h GenList.h ListItr.h ListFunc.h
	$(CC) $(CFLAGS) -c ServerNet.c

ServerManager.o: ServerManager.c ServerManager.h ServerNet.h UserManager.h Limits.h Protocol.h GenVector.h GenList.h ListItr.h ListFunc.h
	$(CC) $(CFLAGS) -c ServerManager.c

Sender.o: Sender.c Sender.h Limits.h
	$(CC) -ansi -g -pedantic -c Sender.c

Listener.o: Listener.c Listener.h Limits.h
	$(CC) -ansi -g -pedantic -c Listener.c

UI.o: UI.c UI.h Limits.h
	$(CC) $(CFLAGS) -c UI.c

ClientNet.o: ClientNet.c ClientNet.h Limits.h
	$(CC) $(CFLAGS) -c ClientNet.c

ClientManager.o: ClientManager.c ClientManager.h ClientNet.h UI.h Limits.h Protocol.h Listener.h Sender.h GenList.h ListItr.h ListFunc.h
	$(CC) $(CFLAGS) -c ClientManager.c

Chat.o: Chat.c Chat.h Limits.h
	$(CC) $(CFLAGS) -c Chat.c
	
ServerManager: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L. -lGenDSLib

ClientManager: $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) -L. -lGenDSLib

Sender: $(OBJS3)
	$(CC) -o $(TARGET3) $(OBJS3)

Listener: $(OBJS4)
	$(CC) -o $(TARGET4) $(OBJS4)

cleanServerManager:
	rm -f $(TARGET) $(OBJS)

cleanClientManager:
	rm -f $(TARGET2) $(OBJS2)

valgrind:
	valgrind --log-file=ServerManager.log ./ServerManager.out
