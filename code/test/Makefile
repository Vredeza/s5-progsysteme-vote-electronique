CC=gcc
CFLAGS=`pkg-config --cflags glib-2.0 sqlite3`
LIBS=`pkg-config --libs glib-2.0 sqlite3` 
BUILDDIR=../build
PROJECTLIBDIR=-L../build/lib
PROJECTLIB=-lvoting


$(shell mkdir -p $(BUILDDIR))

all: $(BUILDDIR)/test_electeur $(BUILDDIR)/test_election $(BUILDDIR)/test_vote $(BUILDDIR)/test_serveur $(BUILDDIR)/test_circular_buffer $(BUILDDIR)/test_serveur_election $(BUILDDIR)/test_circular_buffer_messages


$(BUILDDIR)/test_electeur: test_electeur.c
	$(CC) test_electeur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_electeur

$(BUILDDIR)/test_election: test_election.c
	$(CC) test_election.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_election

$(BUILDDIR)/test_vote: test_vote.c
	$(CC) test_vote.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_vote

$(BUILDDIR)/test_serveur: test_serveur.c
	$(CC) test_serveur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_serveur

$(BUILDDIR)/test_serveur_electeur: test_serveur_electeur.c
	$(CC) test_serveur_electeur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_serveur_electeur

$(BUILDDIR)/test_circular_buffer: test_circular_buffer.c
	$(CC) test_circular_buffer.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_circular_buffer

$(BUILDDIR)/test_serveur_election: test_serveur_election.c
	$(CC) test_serveur_election.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_serveur_election

$(BUILDDIR)/test_circular_buffer_messages: test_circular_buffer_messages.c
	$(CC) test_circular_buffer_messages.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) -o $(LIBS) -o $(BUILDDIR)/test_circular_buffer_messages

clean:
	rm $(BUILDDIR)/test_vote; rm $(BUILDDIR)/test_election; rm $(BUILDDIR)/test_electeur; rm $(BUILDDIR)/test_serveur; rm $(BUILDDIR)/test_circular_buffer; rm $(BUILDDIR)/test_serveur_electeur; rm $(BUILDDIR)/test_serveur_election
