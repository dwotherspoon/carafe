
void setup(void) {
	
}

int main(int argc, char *argv[]) {
	/* Entry point into CGI application. */
	carafe_start();
	setup();
	carafe_run();
	carafe_exit();
	return 0;
}