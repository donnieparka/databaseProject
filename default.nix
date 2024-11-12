{stdenv}:

stdenv.mkderivation {
	pname = "employeesDb";
	version = "v0.0.1";
	src = "./src";
	buildInputs = [./include/*];
	buildPhase = ''
		mkdir -p obj bin
		gcc -c ./src/*.c -o ./obj/$*.o -I include
		gcc -o ./obj/*.o employeesDb
	'';
	installPhase = ''
		cp ./bin/dbView $out/bin
	'';
}
