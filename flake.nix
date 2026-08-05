{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};

      orochi = pkgs.stdenv.mkDerivation {
        pname = "orochi";
        version = "0.1.0";

        src = ./.;

        nativeBuildInputs = [
          crosszgb
        ];

        buildPhase = ''
          export GBDK_HOME=${crosszgb}/gbdk
          export ZGB_PATH=${crosszgb}/ZGB/common
          export HUGEDRIVER=${hugedriver}
          export PATH=$GBDK_HOME/bin:$PATH
          make
        '';

        installPhase = ''
          mkdir -p $out
          cp build/orochi.gb $out/
        '';
      };

      crosszgb = pkgs.stdenv.mkDerivation {
        pname = "crosszgb";
        version = "26.1";

        src = pkgs.fetchzip {
          url = "https://github.com/gbdk-2020/CrossZGB/releases/download/v2026.1/ZGB-Linux-x64.tar.gz";
          hash = "sha256-nzGzVmdGZRemenHoPLDr+4Dp2RgfY6BK3csmJpCOL6o=";
        };

        nativeBuildInputs = with pkgs; [
          autoPatchelfHook
          makeWrapper
        ];

        buildInputs = with pkgs; [
          stdenv.cc.cc.lib
          zlib
        ];

        installPhase = ''
          mkdir -p $out
          cp -r . $out

          wrapProgram $out/gbdk/bin/lcc \
            --set GBDKDIR "$out/gbdk/"
        '';
      };

      hugetrackerGtk2Rc = pkgs.writeText "hugetracker-gtkrc-2.0" ''
        gtk-theme-name = "Materia-dark"
        include "${pkgs.materia-theme}/share/themes/Materia-dark/gtk-2.0/gtkrc"
      '';

      hugetracker = pkgs.stdenv.mkDerivation {
        pname = "hugetracker";
        version = "1.0.11";

        src = pkgs.fetchzip {
          url = "https://github.com/SuperDisk/hUGETracker/releases/download/v1.0.11/hUGETracker-1.0.11-linux.zip";
          sha256 = "sha256-iTZU5N43mHHCSP0y/LgmEqo0YEDhDw/Th+h4bgGqb1k=";
          stripRoot = false;
        };

        nativeBuildInputs = with pkgs; [
          autoPatchelfHook
          makeWrapper
        ];

        buildInputs = with pkgs; [
          SDL2
          libGL
          zlib
          stdenv.cc.cc.lib

          gtk2
          gdk-pixbuf
          pango
          cairo
          atk
          fontconfig
          freetype

          libx11
          libxext
          libxrender
          libxcb
          libxi
          libxcursor
          libxrandr

          gtk-engine-murrine
        ];

        installPhase = ''
          mkdir -p $out/bin
          mkdir -p $out/share/hugetracker

          cp -r . $out/share/hugetracker

          chmod +x $out/share/hugetracker/hUGETracker
          chmod +x $out/share/hugetracker/uge2source

          makeWrapper \
            $out/share/hugetracker/hUGETracker \
            $out/bin/hugetracker \
            --add-flags "--runtime_dir $out/share/hugetracker" \
            --set GTK2_RC_FILES "${hugetrackerGtk2Rc}" \
            --set GTK_PATH "${pkgs.gtk-engine-murrine}"

          makeWrapper \
            $out/share/hugetracker/uge2source \
            $out/bin/uge2source
        '';
      };

      hugedriver = pkgs.stdenv.mkDerivation {
        pname = "hugedriver";
        version = "6.1.3";

        src = pkgs.fetchFromGitHub {
          owner = "SuperDisk";
          repo = "hUGEDriver";
          rev = "v6.1.3";
          hash = "sha256-2zlsyXY5LnLUH3A99SJMq2pcfbolkc7YV7n8058EzP8=";
        };

        nativeBuildInputs = with pkgs; [
          (rgbds.overrideAttrs (old: {
            version = "0.6.1";
            src = fetchFromGitHub {
              owner = "gbdev";
              repo = "rgbds";
              rev = "v0.6.1";
              hash = "sha256-3mx4yymrOQnP5aJCzPWl5G96WBxt1ixU6tdzhhOsF04=";
            };
          }))
          python3
        ];

        buildPhase = ''
          rgbasm -D GBDK -o hUGEDriver.obj hUGEDriver.asm
          python3 tools/rgb2sdas.py -o hUGEDriver.o hUGEDriver.obj
        '';

        installPhase = ''
          mkdir -p $out/include $out/lib
          cp include/hUGEDriver.h $out/include/
          cp hUGEDriver.o $out/lib/
        '';
      };

      bearConfig = pkgs.writeText "bear.yml" ''
        schema: "4.0"
        intercept:
          mode: preload
        compilers:
          - path: ${crosszgb}/gbdk/bin/sdcc
            as: gcc
      '';

      makeWithBear = pkgs.writeShellScriptBin "make" ''
        exec ${pkgs.bear}/bin/bear --config bear.yml --append -- ${pkgs.gnumake}/bin/make "$@"
      '';
    in {
      packages = {
        default = orochi;
        crosszgb = crosszgb;
        hugetracker = hugetracker;
        hugedriver = hugedriver;
      };

      apps.default = {
        type = "app";
        program = "${pkgs.writeShellScript "run-orochi" ''
          exec ${pkgs.gearboy}/bin/gearboy ${orochi}/orochi.gb
        ''}";
      };

      devShells.default = pkgs.mkShell {
        packages = [
          crosszgb
          hugetracker
          hugedriver
          makeWithBear
          pkgs.clang-tools
          pkgs.mbake
          pkgs.bear
          pkgs.gearboy
          pkgs.libresprite
          (pkgs.python3.withPackages (ps: [ps.pillow]))
        ];

        shellHook = ''
          export GBDK_HOME=${crosszgb}/gbdk
          export ZGB_PATH=${crosszgb}/ZGB/common
          export HUGEDRIVER=${hugedriver}
          export PATH=$GBDK_HOME/bin:$PATH
          ln -sf ${bearConfig} bear.yml

          fix-compdb() {
            python3 -c "
              import json, re
              with open('compile_commands.json') as f:
                  db = json.load(f)

              sdcc_only = [
                  '-msm83', '-msm83:gb', '--no-std-crt0', '--fsigned-char',
                  '--use-stdout', '--no-optsdcc-in-asm', '--std-sdcc99',
                  '--codeseg', '--constseg', '--peep-file', '--opt-code-speed',
                  '--model-small', '--no-xinit-opt', '--all-callee-saves',
                  '--stack-auto', '--int-long-reent', '--float-reent',
                  '--out-fmt-ihx', '--debug', '-Wl', '-Wa', '-Wa-pogN'
              ]

              for entry in db:
                  args = entry.get('arguments', [])
                  new_args = []
                  skip_next = False
                  for i, arg in enumerate(args):
                      if skip_next:
                          skip_next = False
                          continue
                      if arg in sdcc_only:
                          continue
                      if arg == '--max-allocs-per-node':
                          skip_next = True  # skip the '50000' that follows
                          continue
                      if arg == '--max-allocs-per-node 50000':
                          continue
                      new_args.append(arg)
                  entry['arguments'] = new_args

              with open('compile_commands.json', 'w') as f:
                  json.dump(db, f, indent=2)
            "
          }
        '';
      };
    });
}
