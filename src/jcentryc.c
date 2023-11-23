// LINT_C_FILE


#ifndef asm
#ifdef __CLANG__
#define asm __asm
#elif __GNUC__
#define asm __asm__
#elif _MSC_VER
#define asm __asm
#endif
#endif

void console_put(const char c) {
	asm (
		//assume number is in eax
		"mov %ecx, 10;"
		"loophere:"
		"mov %edx, 0;"
		"div %ecx;"

		// now eax <-- eax/10
		// edx <-- eax % 10

		// print edx
		// this is one digit, which we have to convert to ASCII
		// the print routine uses edx and eax, so let's push eax
		// onto the stack. we clear edx at the beginning of the
		// loop anyway, so we don't care if we much around with it
		"push %eax;"

		// convert dl to ascii
		"add %dl, '0';"
		"mov %ah, 2;" // 2 is the function number of output char in the DOS Services.
		"int 21;" // calls DOS Services
		// Restore EAX
		"pop %eax;"
		// if eax is zero, we can quit
		"cmp %eax, 0;"
		"jnz loophere;"
	);
}

#ifdef __linux__

extern int main();

void exit(void) {
	asm (
		"movl 1, %eax;"
		"xorl %ebx, %ebx;"
		"int 0x80;"
	);
	__builtin_unreachable();
}

void _start() {
	//
	int code = main();
	if (code > 0) {
		console_put(code & 0xFF);
	}
	exit();
}

#endif
