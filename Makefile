NAME ?= Rogue 

all:
	$(MAKE) -C src all
	mv src/$(NAME) .

clean:
	$(MAKE) -C src clean
	rm -rf *.o $(NAME)
