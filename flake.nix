{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc }:
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

            buildInputs = (with pkgs; [
              gtkmm3
              fmt_9

              # Silence pkg-config warnings
              pcre
              pcre2
              libthai
              libdatrie
              libxkbcommon
              libepoxy
              util-linux
              libselinux
              libsepol
              xorg.libXdmcp
              xorg.libXtst
            ]) ++ [
              tinycmmc.packages.${pkgs.system}.default
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
