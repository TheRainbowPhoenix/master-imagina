typedef int vect<>;

struct minmax_t {
	int min;
	int max;
};

program MINMAX_PROG {
	version MINMAX_VERS {
		minmax_t minmax(vect) = 1;
	} = 1;
} = 1;