all: tablist hlist

tablist: tablist.c
	gcc -o tablist tablist.c

hlist: hlist.c
	gcc -o hlist hlist.c

clean:
	rm -f tablist hlist
