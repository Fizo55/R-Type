{
  description = "Flake config for rtype project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
    in {
      devShells."${system}".default = let
        pkgs = import nixpkgs {
          inherit system;
        };
      in pkgs.mkShell {
        packages = with pkgs; [
          gcc
          cmake
          SDL
          SDL2
          SDL2_image
          pkg-config
          ninja
          yaml-cpp
        ];
        shellHook = ''
          echo Custom C++ nix-shell!
          echo -e "\e[97m"
        '';
      };
    };
}