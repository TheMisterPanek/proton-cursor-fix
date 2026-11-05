# proton-cursor-fix (`fixcursor.so`)

A tiny `LD_PRELOAD` library for Linux / Proton games (specifically tested with **CORDURA Playtest**) where the mouse cursor is invisible in menus because the game or Wine submits a completely transparent 32×32 ARGB cursor image (`0/1024` visible pixels) to the X server.

## How it works

When a Windows game running under Wine/Proton loads a custom ARGB `.cur` file and calls `SetCursor`, `winex11.drv` creates an XRender ARGB cursor via `XRenderCreateCursor` and applies it to the game window via `XDefineCursor`. Meanwhile, Wine's internal monochrome "blank" cursor (used when the game intentionally hides the cursor via `ShowCursor(0)` during gameplay/camera movement) is created via `XCreatePixmapCursor`.

`fixcursor.so` intercepts three functions:
1. **`XRenderCreateCursor`**: Returns a dummy handle (`FAKE_CURSOR`) instead of creating the broken transparent ARGB cursor.
2. **`XDefineCursor`**: Replaces `FAKE_CURSOR` with `None` (so the window falls back to the standard desktop cursor), while allowing Wine's intentional monochrome blank cursor (`XCreatePixmapCursor`) to pass through untouched so the pointer still hides properly during gameplay.
3. **`XFreeCursor`**: Safely ignores `FAKE_CURSOR` on cleanup.

---

## Quick Usage (Pre-built Binary)

Works on any 64-bit Linux distribution and **Steam Deck (SteamOS)**.

1. Download **`fixcursor.so`** from the [**Latest Release**](../../releases/latest) page (for example, into your `~/Downloads` folder).
2. In Steam, right-click the game -> **Properties** -> **Launch Options** and set:
   ```sh
   LD_PRELOAD=$HOME/Downloads/fixcursor.so %command%
   ```
3. Launch the game.

*(Note: Because this replaces custom ARGB hardware cursors with the default desktop cursor, any custom in-game ARGB hardware cursors such as custom crosshairs drawn via XRender will also appear as your system default pointer).*

---

## Building from Source

The library is 27 lines of standard C and only requires X11/Xrender headers at compile time and `libdl` at link time.

### Prerequisites
* **Arch / CachyOS:** `sudo pacman -S gcc libx11 libxrender`
* **Ubuntu / Debian:** `sudo apt install gcc libx11-dev libxrender-dev`
* **Fedora:** `sudo dnf install gcc libX11-devel libXrender-devel`

### Compile
```sh
git clone https://github.com/TheMisterPanek/proton-cursor-fix.git
cd proton-cursor-fix
gcc -shared -fPIC -O2 -o fixcursor.so fixcursor.c -ldl
```

Then point `LD_PRELOAD` in Steam Launch Options to your compiled `fixcursor.so`:
```sh
LD_PRELOAD=/path/to/fixcursor.so %command%
```

---

## License

[MIT License](LICENSE). Feel free to use, modify, and redistribute.
