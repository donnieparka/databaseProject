{pkgs ? import <nixpkgs> {}}:

with pkgs;

stdenv.mkDerivation {
	pname = "employeesDb";
	version = "v0.0.1";
	src = ./.;
	nativeBuildInputs = [ gcc ];
	buildPhase = ''
		mkdir -p obj bin
		for file in src/*.c; do
			name=$(basename $file .c)
			gcc -c $file -o obj/$name.o -I include
		done
		gcc -o  employeesDb obj/*.o
	'';
	installPhase = ''
	 	mkdir -p $out/bin
		cp employeesDb $out/bin
	'';
}
