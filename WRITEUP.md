# WRITEUP

The Raspberry I/O Bug (RIO for short) is in these two snippets of code. 

```c
void printf(const char *s) {
	unsigned int COUNT = 0; // counter
	while (s[COUNT] != '\0') {
		if (s[COUNT] == '\0') {
			return;
		};
		if (s[COUNT] == '\n') {
			NEWLINE(); // newline
			COUNT++; // increment
		};
		video[location++] = s[COUNT++]; // place character
		video[location++] = 0x07; // place attributes
	};
	return;
};
```
```c
void printc(const char *s, int color) {
	unsigned int COUNT = 0; // counter
        while (s[COUNT] != '\0') {
                if (s[COUNT] == '\0') {
                        return; // EOL
                };
                if (s[COUNT] == '\n') {
                        NEWLINE(); // newline
                        COUNT++; // increment
                };
                video[location++] = s[COUNT++]; // place character
                video[location++] = color; // place attributes
        }
        return;
};
```

The bug is that it has a const char, s, but doesn't check it's length before placing it inside of the video buffer for VGA. 

The VGA buffer starts at 0xb8000, so it is possible to overwrite any memory address, past 0xb8000. 

(particularly memory addresses past 0xb87d0, as that is where the buffer ends)

So, you can overwrite any memory; but that requires writing to EVERY memory address before it, starting at 0xb8000. 

The solution is the following:

Step 1. Read the video buffer from 0, to the address to write to - 0xb8000. 

Step 2. Store that in a buffer. 

Step 3. Write to the end of that buffer the byte you want to write. 

Step 4. Run printc, and write each character with an even position, (0, 2, 4, etc.) with the color of the next character (in the odd position, 1, 3, 5, etc.)

Step 5. Clear the screen with clear_screen. 

Step 6 Repeat. 

You can also write to multiple memory addresses by slightly modify that technique. 

Step 1. Read the video buffer from 0, to the highest address to write to - 0xb8000. 

Step 2. Store that in a buffer. 

Step 3. Write to the end of that buffer the byte with the largest address you want to write. 

Step 4. Write to that buffer every byte, with the location IN the buffer being the address of where to write the byte - 0xb8000. 

Step 5. Repeat step 4 until all bytes have been written. 

Step 4. Run printc, and write each character with an even position, (0, 2, 4, etc.) with the color of the next character (in the odd position, 1, 3, 5, etc.)

Step 5. Clear the screen with clear_screen. 

# USAGE

Include the raspberry.c code inside of the kernel, or inside of your own application.

ENSURE YOU ARE ON A KERNEL VERSION LESS THAN 0.0.8, SIMPLY DOWNLOAD 0.0.7, COMPILE IT, AND INSTALL IT. 

Now, you can run w_byte to write to kernel memory. 

Demo:

```c
w_byte(0xf, 'A'); // write to address 0xb8faf (0xb8fa0 + the first argument) the byte 'A'
w_byte(0x41, '\x00') // write to address 0xb8ff0 (0xb8fa0 + the first argument, 0x41) the byte '\x00' (null)

// test if you can patch the kernel
if (!kernel_test()) {
	// if you can't patch the kernel
}
else {
	// if you can patch the kernel
};
```

# Shoutouts

[@billyellis](https://github.com/billyellis), for his awesome YouTube channel on iOS Security Research, which I learned a lot from. 

[@kpwn](https://github.com/kpwn), also known as qwertyoruiop / qwertyoruiopz; for iOS jailbreaks like Yalu and Yalu102. 
