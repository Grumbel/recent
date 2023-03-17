{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = rec {
          default = recent;

          recent = pkgs.stdenv.mkDerivation rec {
            pname = "recent";
            version = "0.0.0";

            src = ./.;

            buildInputs = with pkgs; [
              gtkmm3
              fmt_9
            ];

            nativeBuildInputs = with pkgs; [
              pkg-config
              cmake
            ];
          };
        };
      }
    );
}