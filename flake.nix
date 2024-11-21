{
  description = "databaseProject";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    src = {
      url = "github:donnieparka/databaseProject";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, src }:
    let 
      pkgs = nixpkgs.legacyPackages.x86_64-linux; 
    in {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "databaseProject";
          src = src;
          buildPhase =''
          mkdir -p bin
          for file in src/*.c; do
          name=$(basename $file .c)
          gcc -o bin/$name $file
          done
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp bin/* $out/bin
          '';
        };
  };
}
