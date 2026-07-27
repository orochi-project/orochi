# Orochi

A rhythm game for the original Game Boy.

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
