# pico-freertos

To run a program, first make sure you have picotool installed:
```bash
brew install picotool
```

Also make sure you have set executable permissions build.sh and upload.sh:
```bash
chmod +x build.sh
chmod +x upload.sh
```

Then run:
```bash
./build.sh <exec-name>
# ex:
# ./build.sh blink-test
```
in order to build a program.
Then upload to your RP2350 device using:
```bash
./upload.sh <exec-name>
# ex:
# ./upload.sh blink-test
```