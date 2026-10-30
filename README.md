# NovaOS 0.1

Ein kleines, echtes Hobby-Betriebssystem für x86-PCs (32-Bit-Kernel), das in QEMU
startet. Es ist ein Lernprojekt und **kein Ersatz für Windows**.

Enthalten:
- eigener freistehender C-Kernel
- eigener Multiboot2-Startcode
- VGA-Textausgabe
- einfache PS/2-Tastatureingabe
- Mini-Shell mit `help`, `about`, `clear` und `echo`
- GRUB-Bootmenü und ISO-Build

## Was du brauchst

Windows 11 mit WSL2/Ubuntu. Der Quellcode wird in diesem ZIP geliefert. Die
Werkzeuge (Compiler, GRUB, QEMU) sind externe Programme und werden beim Setup
installiert. Das ist normal: Ein ZIP kann nicht sicher alle Systemwerkzeuge
für deinen Computer mitliefern.

## 1. Einrichten (einmalig)

Öffne Ubuntu (WSL) und wechsle in den Ordner, in den du NovaOS entpackt hast.
Wenn das ZIP zum Beispiel in Downloads liegt:

```bash
cd /mnt/c/Users/DEIN_WINDOWS_BENUTZERNAME/Downloads/NovaOS
```

Installiere die Werkzeuge:

```bash
sudo apt update
sudo apt install -y build-essential gcc-multilib grub-pc-bin grub-common xorriso qemu-system-x86
```

## 2. Bauen und starten

Im Projektordner:

```bash
make
```

Danach:

```bash
make run
```

QEMU öffnet ein virtuelles Gerät und startet `build/NovaOS.iso`.
Zum Beenden von QEMU: `Ctrl` + `Alt` + `G` (je nach QEMU-Fenster auch über das
Fenstermenü).

## 3. Befehle in NovaOS

- `help` – zeigt Befehle
- `about` – Informationen über NovaOS
- `clear` – Bildschirm leeren
- `echo TEXT` – gibt TEXT aus

## Projektstruktur

```text
NovaOS/
├── src/
│   ├── boot.S       # Multiboot2-Header und Einstieg
│   └── kernel.c    # Kernel, Bildschirm, Tastatur und Shell
├── iso/boot/grub/
│   └── grub.cfg    # Bootmenü
├── linker.ld       # Kernel-Speicherlayout
├── Makefile        # Build und QEMU-Start
└── scripts/
    └── setup-ubuntu.sh
```

## Hinweise

- NovaOS ist ein echter, separat bootender Kernel, aber noch ein sehr kleines
  Lern-Betriebssystem.
- Es verwendet derzeit den klassischen VGA-Textmodus und PS/2-Tastaturabfrage.
- Es hat noch kein Dateisystem, Netzwerk, USB-Treiber, Audio, Fenster-System oder
  moderne Grafiktreiber.
- Starte es zunächst **nur in QEMU**. Nicht auf deinem echten PC installieren
  und keine Festplatte formatieren.
- Speichere deine Änderungen regelmäßig in GitHub.
