<img align="left" src="docs/media/logo.png" alt="Logo" width="64" />

> [!NOTE]
> This project is currently being rewritten using [CrossZGB](https://github.com/gbdk-2020/CrossZGB) due to some random (and probably extremely obscure) issues we've encountered upon along the way.
> The result after the rewrite should be much cleaner.

# Orochi

A Cytus-inspired rhythm game for the Game Boy Color.

## Acknowledgements

All credits for the songs in this game belong to the following artists:

1. [Komorebi](https://open.spotify.com/track/5IDaoVWnVNsXpiV9KOm9kI) by [Chasing Mirrors](https://open.spotify.com/artist/5rZuQbaBUI6qi8sB8tBcge)
2. [Dream Flower](https://www.newgrounds.com/audio/listen/950723) by [KLYDIX](https://klydix.newgrounds.com)
3. [Thirdboot](https://onlinesequencer.net/5544280) by [Syllue](https://onlinesequencer.net/members/76174)
4. [Isolation](https://nighthawk22.com/music/isolation/isolation) by [NightHawk22](https://nighthawk22.com)

## Entering the Development Environment

### Native Linux with Nix Flake

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

#### Building and Running in the Nix Environment

To build the program in the Nix environment, first stage all files you want to build. Then, run the command:

```bash
nix build
```

To run the program using Gearboy after building:

```bash
nix run
```
