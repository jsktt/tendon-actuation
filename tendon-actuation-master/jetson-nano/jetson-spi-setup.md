<h1>Setting up SPI on Jetson Nano</h1>

**Note: Need to edit for conciseness**

<h2>Prerequisites</h2>
You will need:

1. A host computer running Linux (Preferably Ubuntu 18.04)
2. A USB cable for flashing the OS onto the Jetson
3. The ``qemu-user-static`` package which can be installed with:
```
    sudo apt-get install qemu-user-static
```
4. The ``device-tree-compiler`` package which can ban be installed with:
```
    sudo apt-get install device-tree-compiler
```

<h2>Building the Root Filesystem</h2>
1. Download the Board Support Package (BSP) and Sample Root Filesystem for the
latest Jetson Linux version that is compatible with the Jetson Nano (32.7.4 at
the time of writing).

2. Extract the BSP and execute the command:

```console
    cd Linux_for_Tegra/rootfs/
```

3. Once inside the rootfs directory, extract the Sample Root Filesystem you
installed to your current directory.
4. Run
```
    cd ..
    sudo ./apply_binaries.sh
```

<h2>Configuring SPI</h2>

5. Find out which dtb file is being applied for your board. This can most
easily be done by flashing the Jetson Nano as described in the section below
and checking the output log for a DTS file. For the Jetson Nano, this will most
likely be either `tegra210-p3448-0000-p3449-0000-a02.dtb` or `tegra210-p3448-0000
-p3449-0000-b00.dtb` depending on the revision of the board.
6. Navigate to ``Linux_for_Tegra/kernel/dtb``. Create a copy of the relevant
device tree file for <ins>your system</ins> for future reference. For example:
```
    cp tegra210-p3448-0000-p3449-0000-XXX.dtb old.dtb
```
7. Decompile the copy by running the command:
```
    dtc -I dtb -O dts old.dts old.dtb
```
8. Open up ``old.dts`` in a text editor and look for ``gpio@6000d00``.
Scroll down to find ``default``. The ``gpio-input`` line might appear as follows:
```
    default {
        gpio-input = <
            TEGRA_GPIO(BB, 0)
            TEGRA_GPIO(B, 4)
            TEGRA_GPIO(C, 0)
            ...
    }
```
The first argument of ``TEGRA_GPIO()`` corresponds to the BANK. Delete all entries
with B or C as the BANK (but not BB or CC).
You might also see a list of hex values instead. The ``TEGRA_GPIO`` macro evaluates
to a hex value and is given by:
```
    TEGRA_GPIO(BANK, PORT) = (BANK << 3) + PORT
```
The numerical value of B and C are 1 and 2, respectively, so you can use this
formula to find the appropriate ``gpio-input`` hex entries to delete.
9. Now search the document for ``spi1_mosi_pc0``. Enable SPI by modifying the
attribute ``nvidia,function`` to ``spi1``. It should now look like:
```
spi1_mosi_pc0 {
    ...
    nvidia,function = "spi1";
    ...
}
```
10. Repeat 9) for the other spi1 pins (they should all be listed right below mosi).
11. Save the file and recompile it by running the command:
```
    dtc -I dts -O dtb -o new.dtb old.dts
```
12. Copy new.dtb to the device tree file for <ins>your system</ins>. e.g:
```
    cp new.dtb tegra210-p3448-0000-p3449-0000-XXX.dtb
```
This new device tree now contains all the necessary modifications to run SPI
on the Jetson Nano.

<h2>Flashing the Jetson Nano</h2>

7. Boot the Jetson Nano into Force Recovery Mode by placing a jumper across
pins 9 and 10 of the J50 pin header, which is located beneath the Jetson Nano
Module (i.e. below the heat sink). Pins 9 and 10 should be labelled GND and
REC, respectively. You can verify that the Jetson is in recovery mode if 1)
the screen remains blank after turning it on or 2) the following entry appears
in the list of USB devices when you run the command ``lsusb``:
```
    Bus <bbb> Device <ddd>: ID 0955: <nnnn> Nvidia Corp.
```
Where ``<bbb>`` and ``<ddd<`` are any 3-digit number and ``<nnnn>`` is either
7c18, 7018.

6. After booting into Force Recovery Mode remove the jumper and connect the
Jetson via USB.
7. Navigate to the Linux_for_Tegra directory and run
```
    sudo ./flash.sh jetson-nano-devkit mmcblk0p1
```
The Jetson Nano will restart once flashing is complete and you can log into
the system as normal.

<h2>Verifying SPI Functionality</h2>
You can verify SPI functionality by checking the output of the command

```
    sudo cat /sys/kernel/debug/tegra_gpio
```

You should see an output similar to

```
Name:Bank:Port CNF OE OUT IN INT_STA INT_ENB INT_LVL
A: 0:0 64 40 40 24 00 00 000000
B: 0:1 f0 00 00 30 00 00 000000
C: 0:2 1f 00 00 00 00 00 000000
D: 0:3 00 00 00 00 00 00 000000
E: 1:0 40 00 00 00 00 00 000000
...
```

If SPI was configured correctly, the B and C line will be all zeros, (excluding
the Name:Bank:Port field).

You can also verify SPi by checking the output of

```
   sudo cat /sys/kernel/debug/tegra_pinctrl_reg | grep -i spi 
```

The output should look like:

```
    Bank: 1 Reg: 0x70003050 Val: 0x0000e044 → spi1_mosi_pc0
    Bank: 1 Reg: 0x70003054 Val: 0x0000e044 → spi1_miso_pc1
    Bank: 1 Reg: 0x70003058 Val: 0x0000e044 → spi1_sck_pc2
    Bank: 1 Reg: 0x7000305c Val: 0x0000e048 → spi1_cs0_pc3
    Bank: 1 Reg: 0x70003060 Val: 0x0000e048 → spi1_cs1_pc4
    ...
```

If SPI was configured correctly, each "Val" should be the same as shown above.

The last test to verify SPI is to run the loopback test, which is described
[here](https://github.com/rm-hull/spidev-test?search=1).

<h2>Remarks</h2>
If you find that SPI stops working after running an apt-upgrade on the Jetson,
the dtb configuration we wrote may have been reverted to default settings by a
package called ``nvidia-l4t-kernel-dtbs``. In this situation, a possible solution is to 
re-flash just the kernel and DTB, which would follow the process described
[here](https://forums.developer.nvidia.com/t/flashing-dtb-on-jetson-tx2-l4t-32-4-4/170479).

**Note: This hasn't been tried yet, so we can't verify that this solution works**
