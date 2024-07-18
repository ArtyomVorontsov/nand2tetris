int Nand(int a, int b);
int Not(int a, int b);
int And(int a, int b);
int Or(int a, int b);
int Xor(int a, int b);
int Mux(int a, int b);
int Dmux(int a, int b);

/* multibit */
int Not16(int a, int b);
int And16(int a, int b);
int Or16(int a, int b);

int Mux16(int a, int b);

/* multiinput */
int Or8Way(int **a);
int Mux4Way16(int a, int b, int c, int d, int sel);
int Mux8Way16(int a, int b, int c, int d, 
		int e, int f, int g, int h, int sel);
int DMux4Way16(int in, int sel);
int DMux4Way16(int in, int sel);


