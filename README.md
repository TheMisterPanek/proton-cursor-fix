# proton-cursor-fix (`fixcursor.so`)

A tiny `LD_PRELOAD` library for Linux / Proton games (specifically tested with **CORDURA Playtest**) where the mouse cursor is invisible in menus because the game or Wine submits a completely transparent 32×32 ARGB cursor image (`0/1024` visible pixels) to the X server.

## How it works

When a Windows game running under Wine/Proton loads a custom `.cur` file and calls `SetCursor`, `winex11.drv` creates an X11 cursor and applies it to the game window via `XDefineCursor`. If the cursor texture is all-transparent, the cursor becomes invisible while mouse clicks still register normally.

`fixcursor.so` intercepts two Xlib functions:
1. **`XCreatePixmapCursor`**: Tracks Wine's internal monochrome "blank" cursor (which Wine uses when the game intentionally hides the cursor via `ShowCursor(0)` during gameplay/camera movement).
2. **`XDefineCursor`**: Replaces any non-blank custom cursor with the standard X11 arrow (`XC_left_ptr`), while allowing Wine's intentional blank cursor to pass through untouched so the pointer still hides properly during gameplay.

---

## Quick Usage (Pre-built Binary)

Works on any 64-bit Linux distribution and **Steam Deck (SteamOS)**.

1. Download **`fixcursor.so`** from the [**Latest Release**](../../releases/latest) page (for example, into your `~/Downloads` folder).
2. In Steam, right-click the game -> **Properties** -> **Launch Options** and set:
   ```sh
   LD_PRELOAD=$HOME/Downloads/fixcursor.so %command%
   ```
3. Launch the game.

*(Note: Because this replaces custom hardware cursors with the standard X11 arrow, any custom in-game hardware cursors such as custom crosshairs drawn via X11 will also appear as an arrow).*

---

## Building from Source

The library is ~45 lines of standard C and only depends on `libdl` and `libX11`.

### Prerequisites
* **Arch / CachyOS:** `sudo pacman -S gcc libx11`
* **Ubuntu / Debian:** `sudo apt install gcc libx11-dev`
* **Fedora:** `sudo dnf install gcc libX11-devel`

### Compile
```sh
git clone https://github.com/TheMisterPanek/proton-cursor-fix.git
cd proton-cursor-fix
gcc -shared -fPIC -O2 -o fixcursor.so fixcursor.c -ldl -lX11
```

Then point `LD_PRELOAD` in Steam Launch Options to your compiled `fixcursor.so`:
```sh
LD_PRELOAD=/path/to/fixcursor.so %command%
```

---

## License

MIT / Public Domain (Unlicense). Feel free to use, modify, and redistribute.
