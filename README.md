# Orochi

A Cytus-inspired rhythm game for the Game Boy Color.

## Acknowledgements

All credits for the songs in this game belong to the following artists:

### Menu Loop

Mellow by v81d: https://onlinesequencer.net/5719933.

This is an original soundtrack (OST) of Orochi.

_Licensed under [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0)._
_You can use, copy, modify, distribute, and perform the work for both commercial and noncommercial purposes. No attribution needed._

### Beatmap Songs

1. [Komorebi](https://open.spotify.com/track/5IDaoVWnVNsXpiV9KOm9kI) by [Chasing Mirrors](https://open.spotify.com/artist/5rZuQbaBUI6qi8sB8tBcge)
2. [Dream Flower](https://www.newgrounds.com/audio/listen/950723) by [KLYDIX](https://klydix.newgrounds.com)
3. [Thirdboot](https://onlinesequencer.net/5544280) by [Syllue](https://onlinesequencer.net/members/76174)
4. [Isolation](https://nighthawk22.com/music/isolation/isolation) by [NightHawk22](https://nighthawk22.com)

## Development Environment

### Linux with Nix Flake

To enter the development environment:

1. Install the [Nix](https://nixos.org/download/#download-nix) package manager.

2. Enable flakes in your per-user Nix configuration at `~/.config/nix/nix.conf`:

```nix
mkdir -p ~/.config/nix
nano ~/.config/nix/nix.conf
```

Put this line inside the file:

```conf
experimental-features = nix-command flakes
```

3. Enter the Nix development at the project root:

```bash
nix develop
```

#### Building and Running in the Nix Environment

To build the program in the Nix environment, first stage all files you want to build. Then, run the command:

```bash
nix build
```

To run the program using Gearboy after building:

```bash
nix run
```

### Windows with WSL (NixOS)

To install all the necessary tools for the development environment, follow the instructions:

1. Install the [Windows Subsystem for Linux (WSL) NixOS](https://github.com/nix-community/NixOS-WSL) distribution.

2. Enter the shell:

```powershell
wsl -d NixOS
```

3. Enable flakes in your per-user Nix configuration at `~/.config/nix/nix.conf`:

```nix
mkdir -p ~/.config/nix
nano ~/.config/nix/nix.conf
```

Put this line inside the file:

```conf
experimental-features = nix-command flakes
```

4. Enter the Nix development at the project root:

```bash
nix develop
```

> [!NOTE]
> You might get an error about the format of the `flake.nix` file. To fix this, enter the command:
>
> ```bash
> sed -i 's/\r$//' flake.nix
> ```

#### Building and Running

To build the program in the Nix environment, first stage all files you want to build. Then, run the command:

```bash
nix build
```

To run the program using Gearboy after building:

```bash
nix run
```

## License

Orochi is free software distributed under the **GNU General Public License, version 3.0 or later (GPL-3.0+).**

You are free to use, modify, and share the software under the terms of the GPL.
For full details, see the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html).

All files in this repository are similarly licensed under the GPL unless otherwise stated.
