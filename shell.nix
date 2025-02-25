{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs =with pkgs; [
    vscode-fhs
    gcc14
    gdb
    pkg-config 
    openssl 
    meson
    ninja
    ];

  shellHook = ''
    echo "Welcome to the  zchain dev environment!"
  '';
}


